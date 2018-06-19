using System;
using System.Diagnostics;
using System.Threading.Tasks;

using Flurl.Http;

namespace PolisNetworkServer
{
	class HTTPHandler
	{
		private string Url;

		public HTTPHandler(string Url)
		{
			this.Url = Url;
		}

		public async Task PostAsync(object Data)
		{
			try
			{
				var response = await Url
							  .PostUrlEncodedAsync(Data)
							  .ReceiveString();
				Debug.WriteLine(response);
			}
			catch (Exception ex)
			{
				Debug.WriteLine(ex.Message);
			}
		}
	}
}