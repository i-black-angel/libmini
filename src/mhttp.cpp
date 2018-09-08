/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <mpl/mhttp.h>
#include <mpl/mlog.h>
#include "mongoose.h"

static void mpl_ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data)
{
    switch (ev) {
	case MG_EV_HTTP_REQUEST:
		struct http_message *msg = (struct http_message *) ev_data;
		mpl::MHttpServer *srv = (mpl::MHttpServer *)user_data;

		// receive message
		std::string message = std::string(msg->message.p, msg->message.len);
		log_debug("%s", message.c_str());

		srv->handler(nc, msg);
		break;
    }
}

MPL_BEGIN_NAMESPACE

MHttp::MHttp()
{
	_mgr = new mg_mgr();
	mg_mgr_init(_mgr, NULL);
}

MHttp::~MHttp()
{
	mg_mgr_free(_mgr);
	delete _mgr;
}

MHttpServer::MHttpServer(int msec)
	: _msec(msec),
	  _interrupt(false)
{
}

MHttpServer::~MHttpServer()
{
}

bool MHttpServer::bind(const std::string &port)
{
	_nc = mg_bind(_mgr, port.c_str(), mpl_ev_handler, this);
	if (_nc == NULL) {
		log_error("bind '%s' failed", port.c_str());
		return false;
	}
	mg_set_protocol_http_websocket(_nc);
	log_debug("Starting web server on port: %s", port.c_str());
	return true;
}

void MHttpServer::start()
{
	while ( !isInterrupted() ) {
		mg_mgr_poll(_mgr, _msec);
	}
}

void MHttpServer::stop()
{
	interrupt();
}

void MHttpServer::handler(struct mg_connection *nc, struct http_message *msg)
{
	std::cout << "handler" << std::endl;
}

MHttpClient::MHttpClient()
{
}

MHttpClient::~MHttpClient()
{
}

MPL_END_NAMESPACE
