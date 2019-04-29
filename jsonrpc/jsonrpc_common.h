/**
 * \file jsonrpc_common.h
 * \brief Common functions and enumerations for JsonRpc-Cpp.
 * \author Sebastien Vincent
 */

#ifndef JSONRPC_COMMON_H
#define JSONRPC_COMMON_H

#ifndef _WIN32

#include <stdint.h>

#else

#ifndef _MSC_VER
#include <stdint.h>
#endif

#endif

namespace Json
{
	namespace Rpc
	{
		/**
		 * \enum ErrorCode
		 * \brief JSON-RPC error codes.
		 * \note Value from -32099 to -32000 are reserved for implementation-defined
		 * server-errors.
		 */
		enum ErrorCode
		{
			PARSING_ERROR = -32700, /**< Invalid JSON. An error occurred on the server while parsing the JSON text. */
			INVALID_REQUEST = -32600, /**< The received JSON not a valid JSON-RPC Request. */
			METHOD_NOT_FOUND = -32601, /**< The requested remote-procedure does not exist / is not available. */
			INVALID_PARAMS = -32602, /**< Invalid method parameters. */
			INTERNAL_ERROR = -32603 /**< Internal JSON-RPC error. */
		};
	} /* namespace Rpc */
} /* namespace Json */

#endif /* JSONRPC_COMMON_H */

