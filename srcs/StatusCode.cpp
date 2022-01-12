/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 11:07:51 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 11:21:00 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StatusCode.hpp"

StatusCode::StatusCode()
{
  _err_code[100] = "Continue";
  _err_code[101] = "Switching Protocols";
  _err_code[103] = "Early Hints";

  _err_code[200] = "OK";
  _err_code[201] = "Created";
  _err_code[202] = "Accepted";
  _err_code[203] = "Non-Authoritative Information";
  _err_code[204] = "No Content";
  _err_code[205] = "Reset Content";
  _err_code[206] = "Partial Content";

  _err_code[300] = "Multiple Choices";
  _err_code[301] = "Moved Permanently";
  _err_code[302] = "Found";
  _err_code[303] = "See Other";
  _err_code[304] = "Not Modified";
  _err_code[305] = "Use Proxy";
  _err_code[307] = "Temporary Redirect";
  _err_code[308] = "Permanent Redirect";

  _err_code[400] = "Bad Request";
  _err_code[401] = "Unauthorized";
  _err_code[402] = "Payment Required";
  _err_code[403] = "Forbidden";
  _err_code[404] = "Not Found";
  _err_code[405] = "Method Not Allowed";
  _err_code[406] = "Not Acceptable";
  _err_code[407] = "Proxy Authentication Required";
  _err_code[408] = "Request Timeout";
  _err_code[409] = "Conflict";
  _err_code[410] = "Gone";
  _err_code[411] = "Length Required";
  _err_code[412] = "Precondition Failed";
  _err_code[413] = "Payload Too Large";
  _err_code[414] = "URI Too Long";
  _err_code[415] = "Unsupported Media Type";
  _err_code[416] = "Range Not Satisfiable";
  _err_code[417] = "Expectation Failed";
  _err_code[418] = "I'm a teapot";
  _err_code[422] = "Unprocessable Entity";
  _err_code[425] = "Too Early";
  _err_code[426] = "Upgrade Required";
  _err_code[428] = "Precondition Required";
  _err_code[429] = "Too Many Requests";
  _err_code[431] = "Request Header Fields Too Large";
  _err_code[451] = "Unavailable For Legal Reasons";

  _err_code[500] = "Internal Server Error";
  _err_code[501] = "Not Implemented";
  _err_code[502] = "Bad Gateway";
  _err_code[503] = "Service Unavailable";
  _err_code[504] = "Gateway Timeout";
  _err_code[505] = "HTTP Version Not Supported";
  _err_code[506] = "Variant Also Negotiates";
  _err_code[507] = "Insufficient Storage";
  _err_code[508] = "Loop Detected";
  _err_code[510] = "Not Extended";
  _err_code[511] = "Network Authentication Required";
}

std::string     &StatusCode::operator[](unsigned int status)
{
    return (_err_code[status]);
}