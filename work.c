#include <stdint.h>
#include "/usr/local/include/mediastreamer2/mscommon.h"
#include "mediastreamer2/msrtp.h"
#include "mediastreamer2/mediastream.h"


struct myStruct{
  MSFilter* soundread;
  MSFilter* soundwrite;
  MSFilter* encoder;  
  MSFilter* decoder;
  MSFilter* rtprecv;
  MSFilter* rtpsned;
  MSFilter* dtmfgn;
};

struct testStruct{
  int i;
};


int main(int argc, char* argv[]){
  ms_init();
  ortp_init();
  RtpSession* temp_session;
  temp_session=rtp_session_new(1);
  myStruct *stream;
  testStruct test1;

  MSSndCard *sndcard;
  sndcard=ms_snd_card_manager_get_default_card(ms_snd_card_manager_get());

  /* audio capture filter */
  MSFilter *soundread=ms_snd_card_create_reader(sndcard);
  MSFilter *soundwrite=ms_snd_card_create_writer(sndcard);

  MSFilter *encoder=ms_filter_create_encoder("PCMU");
  MSFilter *decoder=ms_filter_create_decoder("PCMU");

  MSFilter *rtpsend=ms_filter_new(MS_RTP_SEND_ID);
  MSFilter *rtprecv=ms_filter_new(MS_RTP_RECV_ID);

  RtpSession *rtp_session = temp_session ;

  ms_filter_call_method(rtpsend,MS_RTP_SEND_SET_SESSION,rtp_session);
  ms_filter_call_method(rtprecv,MS_RTP_RECV_SET_SESSION,rtp_session);

  MSFilter *dtmfgen=ms_filter_new(MS_DTMF_GEN_ID);
  int sr = 8000;
  int chan=1;
  ms_filter_call_method(soundread,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(soundwrite,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(stream->encoder,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(stream->decoder,MS_FILTER_SET_SAMPLE_RATE,&sr);

  ms_filter_call_method(soundwrite,MS_FILTER_SET_NCHANNELS, &chan);

 




  /*link the filters and run the graph*/
  ms_filter_link(stream->soundread,0,stream->encoder,0);
  ms_filter_link(stream->encoder,0,stream->rtpsend,0);

  ms_filter_link(stream->rtprecv,0,stream->decoder,0);
  ms_filter_link(stream->decoder,0,stream->dtmfgen,0);
  ms_filter_link(stream->dtmfgen,0,stream->soundwrite,0); 

  /*create tiker*/
  MSTicker *ticker=ms_ticker_new();

  ms_ticker_attach(ticker,soundread);
  ms_ticker_attach(ticker,rtprecv);
  ms_ticker_detach(ticker,soundread);
  ms_ticker_detach(ticker,rtprecv);

  ms_filter_unlink(stream->soundread,0,stream->encoder,0);
  ms_filter_unlink(stream->encoder,0,stream->rtpsend,0);

  ms_filter_unlink(stream->rtprecv,0,stream->decoder,0);
  ms_filter_unlink(stream->decoder,0,stream->dtmfgen,0);
  ms_filter_unlink(stream->dtmfgen,0,stream->soundwrite,0);

  if (rtp_session!=NULL) rtp_session_destroy(rtp_session);
  if (rtpsend!=NULL) ms_filter_destroy(rtpsend);
  if (rtprecv!=NULL) ms_filter_destroy(rtprecv);
  if (soundread!=NULL) ms_filter_destroy(soundread);
  if (soundwrite!=NULL) ms_filter_destroy(soundwrite);
  if (encoder!=NULL) ms_filter_destroy(encoder);
  if (decoder!=NULL) ms_filter_destroy(decoder);
  if (dtmfgen!=NULL) ms_filter_destroy(dtmfgen);
  if (ticker!=NULL) ms_ticker_destroy(ticker);

}
