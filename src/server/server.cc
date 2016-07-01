#include "server.h"

HttpServer::HttpServer()
{
	baseLogger -> commit("Starting base http server");
	this -> mhdDaemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, 
			8888, NULL, NULL,
			&this -> clbHandle, NULL, MHD_OPTION_END);
}

HttpServer::~HttpServer()
{
	baseLogger -> commit("Terminating base http server");
	MHD_stop_daemon(this -> mhdDaemon);
}

void HttpServer::serve()
{
	getchar();
	delete this;
}


void HttpServer::registerApp(applicationHook hook)
{

}


int HttpServer::clbHandle (void *cls, struct MHD_Connection *con,
			const char *url, const char *method,
			const char *version, const char *upload_data,
			size_t *upload_data_size, void **con_cls)
{
	request * curRequest = new request;
	curRequest -> connId = con;
	curRequest -> url = url;
	curRequest -> method = method;
	curRequest -> version = version;
	curRequest -> baseLogger = baseLogger;

	curRequest = HttpServer::defz(curRequest);

  const char *page = curRequest -> resp;
  struct MHD_Response *response;
  int ret;
  
  response =
    MHD_create_response_from_buffer (strlen (page), (void *) page, 
				     MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (con, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}

logger * HttpServer::baseLogger = new logger();
route HttpServer::defz;