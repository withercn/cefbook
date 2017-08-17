#include "include/cef_parser.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "clientScheme.h"
#include "typeMime.h"
#include "utils.h"

namespace {
	std::string scheme_, host_, domain_;

	bool LoadBinaryResource(int binaryId, DWORD& dwSize, LPBYTE& pBytes) {
		HINSTANCE hInst = GetModuleHandle(NULL);
		HRSRC hRes =
			FindResource(hInst, MAKEINTRESOURCE(binaryId), MAKEINTRESOURCE(256));
		if (hRes) {
			HGLOBAL hGlob = LoadResource(hInst, hRes);
			if (hGlob) {
				dwSize = SizeofResource(hInst, hRes);
				pBytes = (LPBYTE)LockResource(hGlob);
				if (dwSize > 0 && pBytes)
					return true;
			}
		}
		return false;
	}
}
clientSchemeHandlerFactory::clientSchemeHandlerFactory() {
	scheme_ = SCHEME_NAME;
	host_ = HOST_NAME;
	domain_ = scheme_.append("://").append(host_);
}
CefRefPtr<CefResourceHandler> clientSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) {
	CEF_REQUIRE_IO_THREAD();
	//const std::string& html_content = "<html><body>Hello!< /body>< / html>";
	//CefRefPtr<CefStreamReader> stream;// =
	//	CefStreamReader::CreateForData(
	//		static_cast<void*>(const_cast<char*>(html_content.c_str())),
	//		html_content.size());
	////auto url = frame->GetURL().c_str();
	////stream->CreateForFile(url);
	//return new CefStreamResourceHandler("text/html", stream);
	std::string url = request->GetURL();
	if (url.substr(strlen(url.c_str()) - 1, 1) == "/")
		url.append("index.html");
	std::string mime_type_ = Utils::GetMimeType(url);
	int len = domain_.length();
	std::string path = url.replace(0, len, Utils::GetApplicationDir().append("/html"));
	path = Utils::UrlDecode(path);
	path = Utils::GetFile(path);
	CefRefPtr<CefStreamReader> reader = CefStreamReader::CreateForFile(path);
	if (!reader.get())
		return NULL;
	return new CefStreamResourceHandler(mime_type_, reader);
	//return new clientSchemeHandler();
}

//clientSchemeHandler::clientSchemeHandler(){
//	offset_ = 0;
//}
//
//bool clientSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) {
//	CEF_REQUIRE_IO_THREAD();
//	bool handled = false;
//	
//	std::string url = request->GetURL();
//	std::string img = domain_.append("/img");
//	//DCHECK(wcsstr(url.c_str(), img.c_str()));
//	if (strstr(url.c_str(), img.c_str())) {
//		//资源为图片目录
//		handled = true;
//		data_ = "<html></html>";
//		mime_type_ = "text/html";
//		CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForFile(img);
//		
//	}
//	else {
//		//资源为html或其它文本
//		handled = true;
//		data_ = "<html></html>";
//		mime_type_ = "text/html";
//	}
//
//	if (handled)
//	{
//		callback->Continue();
//		return true;
//	}
//	return false;
//}
//
//void clientSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) {
//	CEF_REQUIRE_IO_THREAD();
//	DCHECK(!data_.empty());
//
//	response->SetMimeType(mime_type_);
//	response->SetStatus(200);
//
//	// Set the resulting response length
//	response_length = data_.length();
//}
//
//bool clientSchemeHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) {
//	CEF_REQUIRE_IO_THREAD();
//	bool has_data = false;
//	bytes_read = 0;
//
//	if (offset_ < data_.length()) {
//		// Copy the next block of data into the buffer.
//		int transfer_size =
//			std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
//		memcpy(data_out, data_.c_str() + offset_, transfer_size);
//		offset_ += transfer_size;
//
//		bytes_read = transfer_size;
//		has_data = true;
//	}
//
//	return has_data;
//}
//
//void clientSchemeHandler::Cancel() {
//	CEF_REQUIRE_IO_THREAD();
//}