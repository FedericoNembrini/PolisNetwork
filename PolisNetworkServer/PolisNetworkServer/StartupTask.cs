using System;

using Windows.System.Threading;
using Windows.ApplicationModel.Background;

namespace PolisNetworkServer
{
    public sealed class StartupTask : IBackgroundTask
    {
		private static BackgroundTaskDeferral deffereal;

		public async void Run(IBackgroundTaskInstance taskInstance)
		{
			deffereal = taskInstance.GetDeferral();

			MyServer Server = new MyServer();

			await ThreadPool.RunAsync(workItem =>
			{
				Server.Start();
			});
		}
	}
}
