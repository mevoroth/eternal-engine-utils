#include "Import/anim/ImportAnim.hpp"
#include "Animation/Animation.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "File/FileFactory.hpp"
#include "Types/Enums.hpp"
#include "rapidjson/document.h"

namespace Eternal
{
	namespace Import
	{
		using namespace Eternal::Types;

		enum class TimelineType
		{
			TIMELINE_TYPE_FLOAT = 0,
			TIMELINE_TYPE_VECTOR2,
			TIMELINE_TYPE_VECTOR3,
			TIMELINE_TYPE_VECTOR4,
			TIMELINE_TYPE_COUNT
		};

		template<typename TimelineType>
		TimelineType ParseKeyFrameValue(_In_ const rapidjson::Value& InValue);

		template<> float ParseKeyFrameValue(_In_ const rapidjson::Value& InValue)
		{
			return InValue.GetFloat();
		}

		template<> Vector2 ParseKeyFrameValue(_In_ const rapidjson::Value& InValue)
		{
			return Vector2(
				InValue["x"].GetFloat(),
				InValue["y"].GetFloat()
			);
		}

		template<> Vector3 ParseKeyFrameValue(_In_ const rapidjson::Value& InValue)
		{
			return Vector3(
				InValue["x"].GetFloat(),
				InValue["y"].GetFloat(),
				InValue["z"].GetFloat()
			);
		}

		template<> Vector4 ParseKeyFrameValue(_In_ const rapidjson::Value& InValue)
		{
			return Vector4(
				InValue["x"].GetFloat(),
				InValue["y"].GetFloat(),
				InValue["z"].GetFloat(),
				InValue["w"].GetFloat()
			);
		}

		template<typename TimelineType>
		static void ParseKeyFrames(_In_ const char* InTimelineName, _In_ const rapidjson::GenericArray<true, rapidjson::Value>& InKeyFrames, _Inout_ Animation* InOutAnimation)
		{
			AnimationTimeline<TimelineType>& Timeline = InOutAnimation->AddTimeline<TimelineType>(InTimelineName);

			uint32_t KeyFramesCount = InKeyFrames.Size();
			for (uint32_t KeyFrameIndex = 0; KeyFrameIndex < KeyFramesCount; ++KeyFrameIndex)
			{
				AnimationKeyFrame<TimelineType> CurrentKeyFrame = {
					InKeyFrames[KeyFrameIndex]["AnimationKeyTime"].GetFloat(),
					ParseKeyFrameValue<TimelineType>(InKeyFrames[KeyFrameIndex]["AnimationKeyValue"])
				};
				Timeline.AddKeyFrame(CurrentKeyFrame);
			}
		}

		Animation* ImportAnim::Import(_In_ const string& InPath)
		{
			using namespace Eternal::FileSystem;

			if (InPath.size() <= 4 ||
				InPath[InPath.size() - 4] != 'a' ||
				InPath[InPath.size() - 3] != 'n' ||
				InPath[InPath.size() - 2] != 'i' ||
				InPath[InPath.size() - 1] != 'm')
			{
				return nullptr;
			}

			string FullPath = FilePath::Find(InPath, FileType::FILE_TYPE_ANIMATIONS);
			if (FullPath.size() <= 0)
				return nullptr;

			FileContent AnimationContent = LoadFileToMemory(FullPath);

			Animation* AnimationData = new Animation(InPath);

			rapidjson::Document JsonAnimation;
			JsonAnimation.Parse(reinterpret_cast<const char*>(AnimationContent.Content), AnimationContent.Size);

			const auto& Timelines = JsonAnimation["Timelines"].GetArray();
			uint32_t TimelinesCount = Timelines.Size();
			AnimationData->ReserveTimelines(TimelinesCount);

			for (uint32_t TimelineIndex = 0; TimelineIndex < TimelinesCount; ++TimelineIndex)
			{
				const auto& CurrentTimeline = Timelines[TimelineIndex];

				int TimelineType = CurrentTimeline["Type"].GetInt();
				ETERNAL_ASSERT(TimelineType < ToInt(TimelineType::TIMELINE_TYPE_COUNT));

				const char* TimelineName											= CurrentTimeline["Name"].GetString();
				const rapidjson::GenericArray<true, rapidjson::Value>& KeyFrames	= CurrentTimeline["KeyFrames"].GetArray();

				switch (TimelineType)
				{
				case ToInt(TimelineType::TIMELINE_TYPE_FLOAT):
				{
					ParseKeyFrames<float>(TimelineName, KeyFrames, AnimationData);
				} break;
				case ToInt(TimelineType::TIMELINE_TYPE_VECTOR2):
				{
					ParseKeyFrames<Vector2>(TimelineName, KeyFrames, AnimationData);
				} break;
				case ToInt(TimelineType::TIMELINE_TYPE_VECTOR3):
				{
					ParseKeyFrames<Vector3>(TimelineName, KeyFrames, AnimationData);
				} break;
				case ToInt(TimelineType::TIMELINE_TYPE_VECTOR4):
				{
					ParseKeyFrames<Vector4>(TimelineName, KeyFrames, AnimationData);
				} break;
				}
			}

			UnloadFileFromMemory(AnimationContent);

			return AnimationData;
		}
	}
}
