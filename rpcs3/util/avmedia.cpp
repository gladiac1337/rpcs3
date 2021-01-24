#include "util/avmedia.hpp"
#include <string>

namespace avmedia
{
	audio_info::audio_info(const std::string filepath)
	{
		// Only print FFMPEG errors, fatals and panics
		//av_log_set_level(AV_LOG_ERROR);

		this->error = false;
		this->filepath = filepath;

		AVDictionary* av_dict_opts = nullptr;
		av_dict_set(&av_dict_opts, "probesize", "96", 0);
		AVFormatContext* av_format_ctx = nullptr;
		av_format_ctx = avformat_alloc_context();

		// Open input file
		if (avformat_open_input(&av_format_ctx, filepath.c_str(), 0, &av_dict_opts) < 0)
		{
			// Failed to open file
			av_dict_free(&av_dict_opts);
			avformat_free_context(av_format_ctx);
			this->error = true;
			return;
		}
		av_dict_free(&av_dict_opts);

		// Find stream information
		if (avformat_find_stream_info(av_format_ctx, 0) < 0)
		{
			// Failed to load stream information
			avformat_free_context(av_format_ctx);
			this->error = true;
			return;
		}

		// Derive first audio stream id from avformat context
		int stream_index = -1;
		for (uint i = 0; i < av_format_ctx->nb_streams; i++)
		{
			if (av_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				stream_index = i;
				break;
			}
		}
		if (stream_index == -1)
		{
			// Failed to find an audio stream
			avformat_free_context(av_format_ctx);
			this->error = true;
			return;
		}

		AVStream* stream = av_format_ctx->streams[stream_index];
		AVCodecParameters* codec = stream->codecpar;

		this->bitrate = codec->bit_rate / 1000;  // TODO: Assumption, verify value
		this->quantizationBitrate = codec->bit_rate / 1000;  // TODO: Assumption, verify value
		this->samplingRate = codec->sample_rate;  // TODO: Assumption, verify value
		this->duration = av_format_ctx->duration / 1000;  // TODO: Assumption, verify value
		this->releasedYear = 0; // TODO: Use "date" id3 tag for this
		this->codecId = codec->codec_id; // AVCodecID

		AVDictionaryEntry *tag;
		std::string value;

		this->trackNumber = 0;
		tag = av_dict_get(av_format_ctx->metadata, "track", 0, AV_DICT_IGNORE_SUFFIX);
		if (tag != nullptr)
		{
			std::string tmp(tag->value);
			this->trackNumber = stoi(tmp.substr(0, tmp.find("/")));
		}

		tag = av_dict_get(av_format_ctx->metadata, "album", 0, AV_DICT_IGNORE_SUFFIX);
		if (tag != nullptr)
		{
			this->album = tag->value;
		}

		tag = av_dict_get(av_format_ctx->metadata, "title", 0, AV_DICT_IGNORE_SUFFIX);
		if (tag != nullptr)
		{
			this->title = tag->value;
		}

		tag = av_dict_get(av_format_ctx->metadata, "artist", 0, AV_DICT_IGNORE_SUFFIX);
		if (tag != nullptr)
		{
			this->artist = tag->value;
		}

		tag = av_dict_get(av_format_ctx->metadata, "genre", 0, AV_DICT_IGNORE_SUFFIX);
		if (tag != nullptr)
		{
			this->genre = tag->value;
		}


		avformat_close_input(&av_format_ctx);
		avformat_free_context(av_format_ctx);
	}

	bool audio_info::has_error()
	{
		return this->error;
	}

	std::string audio_info::get_filepath()
	{
		return this->filepath;
	}

	int audio_info::get_bitrate()
	{
		return this->bitrate;
	}

	int audio_info::get_quantizationBitrate()
	{
		return this->quantizationBitrate;
	}

	int audio_info::get_samplingRate()
	{
		return this->samplingRate;
	}

	int audio_info::get_duration()
	{
		return this->duration;
	}

	int audio_info::get_releasedYear()
	{
		return this->releasedYear;
	}

	int audio_info::get_codecId()
	{
		return this->codecId;
	}

	int audio_info::get_trackNumber()
	{
		return this->trackNumber;
	}

	std::string audio_info::get_album()
	{
		return this->album;
	}

	std::string audio_info::get_title()
	{
		return this->title;
	}

	std::string audio_info::get_artist()
	{
		return this->artist;
	}

	std::string audio_info::get_genre()
	{
		return this->genre;
	}
}
