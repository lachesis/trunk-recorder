#ifndef P25_RECORDER_H
#define P25_RECORDER_H

#define _USE_MATH_DEFINES

#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <gnuradio/io_signature.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/fir_filter_ccf.h>
#include <gnuradio/filter/fir_filter_fff.h>
#include <gnuradio/filter/freq_xlating_fir_filter_ccf.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/rational_resampler_base_ccc.h>

#include <gnuradio/analog/quadrature_demod_cf.h>

#include <gnuradio/analog/sig_source_c.h>
#include <gnuradio/analog/feedforward_agc_cc.h>

#include <gnuradio/digital/diff_phasor_cc.h>

#include <gnuradio/blocks/complex_to_arg.h>


#include <gnuradio/blocks/multiply_cc.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/blocks/multiply_const_cc.h>

#include <gnuradio/blocks/file_sink.h>
#include <gnuradio/filter/pfb_arb_resampler_ccf.h>
#include <gnuradio/filter/rational_resampler_base_ccf.h>
#include <gnuradio/filter/rational_resampler_base_fff.h>

#include <gnuradio/block.h>
#include <gnuradio/blocks/null_sink.h>

#include <gnuradio/blocks/copy.h>

#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/char_to_float.h>
#include <op25/decoder_bf.h>
#include <op25/fsk4_demod_ff.h>
#include <op25/fsk4_slicer_fb.h>
#include <op25_repeater/fsk4_slicer_fb.h>
#include <op25_repeater/p25_frame_assembler.h>
#include <op25_repeater/gardner_costas_cc.h>
#include <op25_repeater/vocoder.h>

#include <gnuradio/msg_queue.h>
#include <gnuradio/message.h>

#include <gnuradio/blocks/head.h>

#include "nonstop_wavfile_sink.h"
#include <gnuradio/blocks/file_sink.h>
#include "recorder.h"
#include "smartnet.h"

class Source;
class p25_recorder;
typedef boost::shared_ptr<p25_recorder> p25_recorder_sptr;
p25_recorder_sptr make_p25_recorder(Source *src, long t, int n);
#include "source.h"

class p25_recorder : public gr::hier_block2, public Recorder
{
	friend p25_recorder_sptr make_p25_recorder(Source *src, long t, int n);
protected:
	p25_recorder(Source *src, long t, int n);

public:
	~p25_recorder();

	void tune_offset(double f);
	void activate( long talkgroup, double f, int num, char *existing_filename);

	void deactivate();
	double get_freq();
	bool is_active();
	int lastupdate();
	long elapsed();
	void mute();
	void unmute();
	char *get_filename();
    Source *get_source();
	gr::msg_queue::sptr tune_queue;
	gr::msg_queue::sptr traffic_queue;
	gr::msg_queue::sptr rx_queue;
	//void forecast(int noutput_items, gr_vector_int &ninput_items_required);

private:
	double center, freq;
	bool muted;
	long talkgroup;
	time_t timestamp;
	time_t starttime;

        Source *source;
	char filename[160];
	char raw_filename[160];
	int num;

	bool iam_logging;
	bool active;


	std::vector<float> lpf_coeffs;
	std::vector<float> arb_taps;
	std::vector<float> sym_taps;

	/* GR blocks */
	gr::filter::fir_filter_ccf::sptr lpf;
	gr::filter::fir_filter_fff::sptr sym_filter;

	gr::analog::sig_source_c::sptr lo;

gr::digital::diff_phasor_cc::sptr diffdec;

	gr::blocks::multiply_cc::sptr mixer;
	gr::blocks::file_sink::sptr fs;

	gr::filter::pfb_arb_resampler_ccf::sptr arb_resampler;
	gr::filter::freq_xlating_fir_filter_ccf::sptr prefilter;
	gr::filter::rational_resampler_base_ccf::sptr downsample_sig;
	gr::filter::rational_resampler_base_fff::sptr upsample_audio;

	gr::analog::quadrature_demod_cf::sptr fm_demod;
	gr::analog::feedforward_agc_cc::sptr agc;

	gr::blocks::nonstop_wavfile_sink::sptr wav_sink;

	gr::blocks::short_to_float::sptr converter;
	gr::blocks::copy::sptr valve;

	gr::blocks::multiply_const_ff::sptr multiplier;
	gr::blocks::multiply_const_ff::sptr rescale;
	gr::blocks::multiply_const_ff::sptr baseband_amp;
	gr::blocks::complex_to_arg::sptr to_float;
	gr::op25::fsk4_demod_ff::sptr fsk4_demod;
	gr::op25_repeater::p25_frame_assembler::sptr op25_frame_assembler;

	gr::op25_repeater::fsk4_slicer_fb::sptr slicer;
	gr::op25_repeater::vocoder::sptr op25_vocoder;
	gr::op25_repeater::gardner_costas_cc::sptr costas_clock;
};


#endif

