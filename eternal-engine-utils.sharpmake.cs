using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine-project.sharpmake.cs")]

namespace EternalEngine
{
	[Sharpmake.Generate]
	public class EternalEngineUtilsProject : EternalEngineBaseProject
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
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-extern\libtga",
				@"$(SolutionDir)eternal-engine-extern\rapidjson\include",
				@"$(SolutionDir)eternal-engine-extern\DirectXTex",
				@"$(SolutionDir)eternal-engine-shaders",
				EternalEngineSettings.FBXSDKPath + @"\include",
			});

			if (!ExtensionMethods.IsPC(InTarget.Platform))
			{
				InConfiguration.IncludePaths.AddRange(new string[] {
					@"$(SolutionDir)eternal-engine-graphics\GraphicsPrivate\include",
				});
			}

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"FBXSDK_SHARED=(ETERNAL_PLATFORM_WINDOWS)",
			});
		}
	}
}
