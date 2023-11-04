using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine.sharpmake.cs")]

namespace EternalEngine
{
	[Sharpmake.Generate]
	public class EternalEngineUtilsProject : EternalEngineProject
	{
		public EternalEngineUtilsProject()
			: base(
				"utils",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader |
					EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, Target InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);

			// Include paths
			InConfiguration.IncludePaths.Add(new string[] {
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-extern\libtga",
				@"$(SolutionDir)eternal-engine-extern\json",
				@"$(SolutionDir)eternal-engine-extern\DirectXTex",
				@"$(SolutionDir)eternal-engine-shaders",
				EternalEngineSettings.FBXSDKPath + @"\include",
			});

			// Defines
			InConfiguration.Defines.Add(new string[] {
				"FBXSDK_SHARED=1",
			});
		}
	}
}
