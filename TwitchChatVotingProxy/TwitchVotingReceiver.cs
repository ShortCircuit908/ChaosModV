using Serilog;
using System;
using System.Threading.Tasks;
using TwitchChatVotingProxy.BitsReceiver;
using TwitchChatVotingProxy.SubReceiver;
using TwitchLib.Client;
using TwitchLib.Client.Enums;
using TwitchLib.Client.Events;
using TwitchLib.Client.Models;
using TwitchLib.Communication.Clients;
using TwitchLib.Communication.Events;
using TwitchLib.PubSub;
using TwitchLib.PubSub.Events;

namespace TwitchChatVotingProxy.VotingReceiver
{
    /// <summary>
    /// Twitch voting receiver implementation
    /// </summary>
    class TwitchVotingReceiver : IVotingReceiver, ISubReceiver, IBitsReceiver
    {
        public static readonly int RECONNECT_INTERVAL = 1000;

        public event EventHandler<OnMessageArgs> OnMessage;
        public event EventHandler<OnSubArgs> OnSubscription;
        public event EventHandler<OnBitsArgs> OnBits;

        private TwitchClient client;
        private TwitchPubSub clientPubSub;
        private TwitchVotingReceiverConfig config;
        private ILogger logger = Log.Logger.ForContext<TwitchVotingReceiver>();

        public TwitchVotingReceiver(TwitchVotingReceiverConfig config)
        {
            this.config = config;

            clientPubSub.Connect();

            // Connect to twitch
            logger.Information(
                $"trying to connect to channel \"{config.ChannelName}\" with user \"{config.UserName}\""
            );

            client = new TwitchClient(new WebSocketClient());
            client.Initialize(
                new ConnectionCredentials(config.UserName, config.OAuth),
                config.ChannelName
            );
            client.OnReSubscriber += OnReSubscribed;
            client.OnNewSubscriber += OnNewSubscribed;
            client.OnGiftedSubscription += OnGiftedSubscription;
            client.OnContinuedGiftedSubscription += OnContinuedGiftedSubscription;
            client.OnCommunitySubscription += OnCommunitySubscribed;
            client.OnConnected += OnConnected;
            client.OnError += OnError;
            client.OnIncorrectLogin += OnIncorrectLogin;
            client.OnJoinedChannel += OnJoinedChannel;
            client.OnMessageReceived += OnMessageReceived;

            client.Connect();

            clientPubSub.OnPubSubServiceConnected += onPubSubServiceConnected;
            clientPubSub.OnBitsReceived += onBitsReceived;
        }

        public void SendMessage(string message)
        {
            try
            {
                client.SendMessage(config.ChannelName, message);
            } catch (Exception e)
            {
                logger.Error(e, $"failed to send message to channel \"{config.ChannelName}\"");
            }
        }
        /// <summary>
        /// Called when the twitch client connects (callback)
        /// </summary>
        private void OnConnected(object sender, OnConnectedArgs e)
        {
            logger.Information("successfully connected to twitch");
        }
        /// <summary>
        /// Called when the twitch client disconnects (callback)
        /// </summary>
        private async void OnDisconnect(object sender, OnDisconnectedArgs e)
        {
            logger.Error("disconnected from the twitch channel, trying to reconnect");
            await Task.Delay(RECONNECT_INTERVAL);
            client.Connect();
        }
        /// <summary>
        /// Called when the twitch clients errors (callback)
        /// </summary>
        private void OnError(object sender, OnErrorEventArgs e)
        {
            logger.Error(e.Exception, "client error, disconnecting");
            client.Disconnect();
        }
        /// <summary>
        /// Called when the twitch client has an failed login attempt (callback)
        /// </summary>
        private void OnIncorrectLogin(object sender, OnIncorrectLoginArgs e)
        {
            logger.Error("incorrect twitch login, check user name and oauth");
            client.Disconnect();
        }
        /// <summary>
        /// Called when the twitch client joins a channel (callback)
        /// </summary>
        private void OnJoinedChannel(object sender, OnJoinedChannelArgs e)
        {
            logger.Information($"successfully joined twitch channel \"{config.ChannelName}\"");
            clientPubSub.ListenToBitsEvents(e.Channel);
            logger.Information($"listening for bits in twitch channel \"{config.ChannelName}\"");
        }
        /// <summary>
        /// Called when the twitch client receives a message
        /// </summary>
        private void OnMessageReceived(object sender, OnMessageReceivedArgs e)
        {
            var chatMessage = e.ChatMessage;

            var evnt = new OnMessageArgs();
            evnt.Message = chatMessage.Message.Trim();
            evnt.ClientId = chatMessage.UserId;
            OnMessage.Invoke(this, evnt);
        }
        private void OnReSubscribed(object sender, OnReSubscriberArgs e)
        {
            HandleSubscribe(e.ReSubscriber);
        }

        private void OnNewSubscribed(object sender, OnNewSubscriberArgs e)
        {
            HandleSubscribe(e.Subscriber);
        }

        private void OnGiftedSubscription(object sender, OnGiftedSubscriptionArgs e)
        {
            HandleSubscribe(e.GiftedSubscription.MsgParamRecipientId, e.GiftedSubscription.DisplayName, e.GiftedSubscription.MsgParamSubPlan);
        }

        private void OnContinuedGiftedSubscription(object sender, OnContinuedGiftedSubscriptionArgs e)
        {
            HandleSubscribe(e.ContinuedGiftedSubscription.UserId, e.ContinuedGiftedSubscription.DisplayName, SubscriptionPlan.NotSet); // TODO: Find correct subscription level
        }

        private void OnCommunitySubscribed(object sender, OnCommunitySubscriptionArgs e)
        {
            HandleSubscribe(e.GiftedSubscription.UserId, e.GiftedSubscription.DisplayName, e.GiftedSubscription.MsgParamSubPlan);
        }

        private void HandleSubscribe(SubscriberBase subscriber)
        {
            HandleSubscribe(subscriber.UserId, subscriber.DisplayName, subscriber.SubscriptionPlan);
        }

        private void HandleSubscribe(string userId, string userName, SubscriptionPlan plan)
        {
            var evnt = new OnSubArgs();
            evnt.Tier = Enum.GetName(typeof(SubscriptionPlan), plan);
            evnt.ClientId = userId;
            evnt.ClientName = userName;
            OnSubscription.Invoke(this, evnt);
            OnSubscription.Invoke(this, evnt);
        }

        private void onPubSubServiceConnected(object sender, EventArgs args)
        {
            logger.Information("successfully connected to PubSub service");
        }

        private void onBitsReceived(object sender, OnBitsReceivedArgs args)
        {
            var evnt = new OnBitsArgs();
            evnt.ClientId = args.UserId;
            evnt.Bits = args.BitsUsed;
            OnBits.Invoke(this, evnt);
        }
    }
}
