using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TwitchChatVotingProxy.SubReceiver
{
    class OnSubArgs
    {
        public string ClientId { get; set; }
        public string ClientName { get; set; }
        public string Tier { get; set; }
    }
}
