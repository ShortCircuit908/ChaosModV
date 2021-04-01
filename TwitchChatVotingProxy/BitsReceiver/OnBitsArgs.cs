using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TwitchChatVotingProxy.BitsReceiver
{
    class OnBitsArgs
    {
        public string ClientId { get; set; }
        public int Bits { get; set; }
    }
}
