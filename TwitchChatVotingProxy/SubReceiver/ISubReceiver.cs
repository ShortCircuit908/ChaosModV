using System;

namespace TwitchChatVotingProxy.SubReceiver
{
    /// <summary>
    /// Defines the interface that a subscription receiver needs to satisfy
    /// </summary>
    interface ISubReceiver
    {
        /// <summary>
        /// Events which get invoked when the sub receiver receives a subscription
        /// </summary>
        event EventHandler<OnSubArgs> OnSubscription;
        /// <summary>
        /// Sends a message to the connected service
        /// </summary>
        /// <param name="message">Message that should be sent</param>
        void SendMessage(string message);
    }
}
