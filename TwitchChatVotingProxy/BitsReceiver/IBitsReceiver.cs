using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TwitchLib.Client.Events;

namespace TwitchChatVotingProxy.BitsReceiver
{
    /// <summary>
    /// Defines the interface that a bits receiver needs to satisfy
    /// </summary>
    interface IBitsReceiver
    {
        /// <summary>
        /// Events which get invoked when the bits receiver receives bits
        /// </summary>
        event EventHandler<OnBitsArgs> OnBits;
        /// <summary>
        /// Sends a message to the connected service
        /// </summary>
        /// <param name="message">Message that should be sent</param>
        void SendMessage(string message);
    }
}
