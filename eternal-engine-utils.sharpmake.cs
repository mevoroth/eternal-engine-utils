using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine-project.sharpmake.cs")]

namespace EternalEngine
{
	public class EternalEngineUtilsProjectUtils
	{
		public static void ConfigureAll(Project.Configuration InConfiguration, ITarget InTarget)
		{
			// Include paths
			InConfiguration.IncludePaths.AddRange(new string[] {
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-graphics\include",
				@"$(SolutionDir)eternal-engine-graphics\GraphicsPrivate\include",
				@"$(SolutionDir)eternal-engine-extern\libtga",
				@"$(SolutionDir)eternal-engine-extern\libwav",
				@"$(SolutionDir)eternal-engine-extern\rapidjson\include",
				@"$(SolutionDir)eternal-engine-extern\DirectXTex",
				@"$(SolutionDir)eternal-engine-shaders",
				EternalEngineSettings.FBXSDKPath + @"\include",
			});

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"FBXSDK_SHARED=(ETERNAL_PLATFORM_WINDOWS)",
			});
		}
	}

	[Sharpmake.Generate]
	public class EternalEngineUtilsProject : EternalEngineBaseProject
	{
		public EternalEngineUtilsProject()
			: base(
				typeof(Target),
				"utils",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader |
					EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineUtilsProjectUtils.ConfigureAll(InConfiguration, InTarget);
		}
	}

	[Sharpmake.Generate]
	public class EternalEngineUtilsAndroidProject : EternalEngineBaseAndroidProject
	{
		public EternalEngineUtilsAndroidProject()
			: base(
				typeof(AndroidTarget),
				"utils",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader |
					EternalEngineProjectSettingsFlags.EEPSF_IncludeHLSLReflection
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineUtilsProjectUtils.ConfigureAll(InConfiguration, InTarget);
		}
	}
}
