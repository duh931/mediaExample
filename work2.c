#include <stdint.h>
#include "/usr/local/include/mediastreamer2/mscommon.h"
#include "mediastreamer2/msrtp.h"
#include "mediastreamer2/mediastream.h"


/*this document tries to enable your own ortp session
and let the ortp session pass data streams (voice and video) 
to target. Right now the code can not define a varaible "stream",
which is explicitly used in the official document. Still looking
for a solution*/


int main(int argc, char* argv[]){
  ms_init();
  ortp_init();
  RtpSession* temp_session;
  temp_session=rtp_session_new(1);

  MSSndCard *sndcard;
  sndcard=ms_snd_card_manager_get_default_card(ms_snd_card_manager_get());

  /*define sample audio capture filter */
  MSFilter *soundread=ms_snd_card_create_reader(sndcard);
  MSFilter *soundwrite=ms_snd_card_create_writer(sndcard);

  MSFilter *encoder=ms_filter_create_encoder("PCMU");
  MSFilter *decoder=ms_filter_create_decoder("PCMU");

  MSFilter *rtpsend=ms_filter_new(MS_RTP_SEND_ID);
  MSFilter *rtprecv=ms_filter_new(MS_RTP_RECV_ID);

  RtpSession *rtp_session = temp_session ;

  ms_filter_call_method(rtpsend,MS_RTP_SEND_SET_SESSION,rtp_session);
  ms_filter_call_method(rtprecv,MS_RTP_RECV_SET_SESSION,rtp_session);

/*define capture rate and initialize filters*/
  MSFilter *dtmfgen=ms_filter_new(MS_DTMF_GEN_ID);
  int sr = 8000;
  int chan=1;
  ms_filter_call_method(soundread,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(soundwrite,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(encoder,MS_FILTER_SET_SAMPLE_RATE,&sr);
  ms_filter_call_method(decoder,MS_FILTER_SET_SAMPLE_RATE,&sr);

  ms_filter_call_method(soundwrite,MS_FILTER_SET_NCHANNELS, &chan);

 




  /*link the filters and run the graph*/
  ms_filter_link(soundread,0, encoder,0);
  ms_filter_link( encoder,0, rtpsend,0);

  ms_filter_link( rtprecv,0, decoder,0);
  ms_filter_link( decoder,0, dtmfgen,0);
  ms_filter_link( dtmfgen,0, soundwrite,0); 

  /*create tiker*/
  MSTicker *ticker=ms_ticker_new();

  ms_ticker_attach(ticker,soundread);
  ms_ticker_attach(ticker,rtprecv);
  ms_ticker_detach(ticker,soundread);
  ms_ticker_detach(ticker,rtprecv);

/*unlink filters and destroy the MS objects*/
  ms_filter_unlink( soundread,0, encoder,0);
  ms_filter_unlink( encoder,0, rtpsend,0);

  ms_filter_unlink( rtprecv,0, decoder,0);
  ms_filter_unlink( decoder,0, dtmfgen,0);
  ms_filter_unlink( dtmfgen,0, soundwrite,0);
  

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
