#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QFile>
#include <cstdlib>
#include "utils.h"

const QString kTempFile = "__temp__.json";
const QString kAnalyzeBaseCmd = "ffprobe -v error -show_entries ";
// Probe file formats
const QString kAnalyzeFileCmd = kAnalyzeBaseCmd + \
        "format=format_name,duration,size -of json ";
// Probe stream formats
const QString kAnalyzeStreamBaseCmd = kAnalyzeBaseCmd + \
        "stream=codec_name,bit_rate,size,duration";
const QString kAnalyzeAudioStreamCmd = kAnalyzeStreamBaseCmd + \
        ",sample_rate,channels -of json ";
const QString kAnalyzeVideoStreamCmd = kAnalyzeStreamBaseCmd + \
        ",width,height,frame_rate,pix_fmt -of json ";
// Probe packet formats
const QString kGetPacketBaseCmd = kAnalyzeBaseCmd + \
        "packet=duration_time,dts_time,size,flags ";
const QString kGetAudioPacketsCmd = kGetPacketBaseCmd + \
        "-select_streams a:0 -of json ";
const QString kGetVideoPacketsCmd = kGetPacketBaseCmd + \
        "-select_streams v:0 -of json ";
// End of command
const QString kCmdSuffix = " > " + kTempFile;

class MediaFile
{
public:
    MediaFile(QString path):
        mPath(path) {
        AnalyzeFile();
        AnalyzeAudio();
        AnalyzeVideo();
    }

    ~MediaFile() {
        QFile tempFile(kTempFile);
        if (tempFile.exists()) {
            tempFile.remove();
        }
    }

    bool AnalyzeFile() {
        QString analyzeCmd = kAnalyzeFileCmd + mPath + kCmdSuffix;
        system(analyzeCmd.toLatin1().data());
        if (QFile::exists(kTempFile)) {
            QJsonParseError error;
            QJsonDocument doc = JsonParser::loads(kTempFile, error);
            if ((doc.isObject()) && (error.error == QJsonParseError::NoError)) {
                QJsonObject format = doc.object().value(QString("format")).toObject();
                mFileMeta.setMeta(format);
                return true;
            }
        }
        return false;
    }

    bool AnalyzeAudio() {
        QString analyzeCmd = kAnalyzeAudioStreamCmd + mPath + kCmdSuffix;
        system(analyzeCmd.toLatin1().data());
        if (QFile::exists(kTempFile)) {
            QJsonParseError error;
            QJsonDocument doc = JsonParser::loads(kTempFile, error);
            if ((doc.isObject()) && (error.error == QJsonParseError::NoError)) {
                QJsonObject stream = doc.object().value(QString("streams")).\
                        toArray().at(0).toObject();
                mAudioMeta.setMeta(stream);
                return true;
            }
        }
        return false;
    }

    bool AnalyzeVideo() {
        QString analyzeCmd = kAnalyzeVideoStreamCmd + mPath + kCmdSuffix;
        system(analyzeCmd.toLatin1().data());
        if (QFile::exists(kTempFile)) {
            QJsonParseError error;
            QJsonDocument doc = JsonParser::loads(kTempFile, error);
            if ((doc.isObject()) && (error.error == QJsonParseError::NoError)) {
                QJsonObject stream = doc.object().value(QString("streams")).\
                        toArray().at(0).toObject();
                mVideoMeta.setMeta(stream);
                return true;
            }
        }
        return false;
    }

private:
    class FileMeta {
    public:
        FileMeta():
            mFormatName(QString()),
            mDuration(0),
            mSize(0){
        }

        void setMeta(QJsonObject format) {
            mFormatName = JsonParser::getValueStr(format, "format_name");
            mDuration = JsonParser::getValueStr(format, "duration").toFloat();
            mSize = JsonParser::getValueStr(format, "size").toLongLong();
        }
    private:
        QString mFormatName;
        float mDuration;
        long long mSize;
    };

    class StreamMeta {
    public:
        StreamMeta():
            mCodec(QString()),
            mSize(0),
            mBitRate(0){
        }
        void setMeta(QJsonObject stream) {
            mCodec = JsonParser::getValueStr(stream, "codec_name");
            mSize = JsonParser::getValueStr(stream, "size").toLongLong();
            mBitRate = JsonParser::getValueStr(stream, "bit_rate").toFloat();
        }
    protected:
        QString mCodec;
        long long mSize;
        float mBitRate;
    };

    class AudioMeta: public StreamMeta {
    public:
        AudioMeta():
            mSampleRate(0),
            mChannels(0) {
        }

        void setMeta(QJsonObject stream) {
            StreamMeta::setMeta(stream);
            mSampleRate = JsonParser::getValueStr(stream, "sample_rate").toInt();
            mChannels = JsonParser::getValueStr(stream, "channels").toInt();
        }
    private:
        int mSampleRate;
        int mChannels;
    };

    class VideoMeta : public StreamMeta {
    public:
        VideoMeta():
            mWidth(0),
            mHeight(0),
            mFrameRate(0),
            mPixFmt(QString()){
        }

        void setMeta(QJsonObject stream) {
            StreamMeta::setMeta(stream);
            mWidth = JsonParser::getValueStr(stream, "width").toInt();
            mHeight = JsonParser::getValueStr(stream, "height").toInt();
            mFrameRate = JsonParser::getValueStr(stream, "frame_rate").toFloat();
            mPixFmt = JsonParser::getValueStr(stream, "pix_fmt");
        }
    private:
        int mWidth;
        int mHeight;
        float mFrameRate;
        QString mPixFmt;
    };

    class VideoPackets {
    public:
        VideoPackets() {

        }
    private:

    };

    QString mPath;
    FileMeta mFileMeta;
    AudioMeta mAudioMeta;
    VideoMeta mVideoMeta;
};

#endif // MEDIAFILE_H
