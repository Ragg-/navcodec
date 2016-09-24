import native from '../build/Release/navcodec.node';

export default class Stream
{
    constructor(media)
    {
        this._media = media;

        this._stream = new native.NAVStreamReader(media.inputFile);
        console.log(this._stream.setMetaData({
            width: media.width,
            height:media.height,
            framerate: media.videoStreams[0] ? media.videoStreams[0].codec.framerate : 0,
        }));
    }

    async next() {
        return this._stream.next();
    }
}
