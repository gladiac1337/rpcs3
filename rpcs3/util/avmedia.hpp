#pragma once

#include "stdafx.h"
#include <string>

#include "Utilities/StrUtil.h"

#ifdef _MSC_VER
#pragma warning(push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/dict.h"
}

namespace avmedia
{
/*	enum class av_type : unsigned
	{
		audio, // Highest log severity (cannot be disabled)
		video,
		picture,
	};
*/
	class audio_info
	{
		bool error;
		std::string filepath;
		int bitrate;
		int quantizationBitrate;
		int samplingRate;
		int duration;
		int releasedYear;
		int codecId;
		int trackNumber;
		std::string album;
		std::string title;
		std::string artist;
		std::string genre;

	public:
		bool has_error();
		audio_info(const std::string filepath);
		std::string get_filepath();
		int get_bitrate();
		int get_quantizationBitrate();
		int get_samplingRate();
		int get_duration();
		int get_releasedYear();
		int get_codecId();
		int get_trackNumber();
		std::string get_album();
		std::string get_title();
		std::string get_artist();
		std::string get_genre();
	};
}

//LOG_CHANNEL(rsx_log, "AVMEDIA");
