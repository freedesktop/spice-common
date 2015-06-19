/*
   Copyright (C) 2015 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <common/marshaller.h>
#include <common/generated_server_marshallers.h>
#include <common/client_demarshallers.h>

#define NUM_CHANNELS 3u

int main(void)
{
	SpiceMarshaller *m;
	SpiceMsgChannels *msg;
	uint8_t *data, *out;
	size_t len;
	int to_free = 0;
	spice_parse_channel_func_t func;
	unsigned int max_message_type, n;
	message_destructor_t free_output;

	m = spice_marshaller_new();
	assert(m);

	msg = (SpiceMsgChannels *) malloc(sizeof(SpiceMsgChannels) +
	      NUM_CHANNELS * sizeof(SpiceChannelId));
	assert(msg);

	// build a message and marshal it
	msg->num_of_channels = NUM_CHANNELS;
	for (n = 0; n < NUM_CHANNELS; ++n)
		msg->channels[n] = (SpiceChannelId) { n + 1, n * 7 };
	spice_marshall_msg_main_channels_list(m, msg);

	// get linear data
	data = spice_marshaller_linearize(m, 0, &len, &to_free);
	assert(data);

	printf("output len %lu\n", (unsigned long) len);

	// hack, try to core
	*((uint32_t *) data) = 0x80000002u;

	// extract the message
	func = spice_get_server_channel_parser(1, &max_message_type);
	assert(func);
	out = func(data, data+len, SPICE_MSG_MAIN_CHANNELS_LIST, 0, &len, &free_output);
	assert(out == NULL);

	// cleanup
	if (to_free)
		free(data);
	if (out)
		free_output(out);
	free(msg);

	return 0;
}

