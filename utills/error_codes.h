//
// Created by egor on 02.06.2020.
//

#ifndef SCREAMER_UTILLS_ERROR_CODES_H
#define SCREAMER_UTILLS_ERROR_CODES_H

enum ServerErrorCode
{
	SERVER_NOERROR,
	SERVER_READ_CONFIG_ERROR,
	SERVER_RECVMSG_ERROR,
	SERVER_CREATECOMMAND_ERROR,
	SERVER_UPLOAD_ERROR,
	SERVER_DOWNLOAD_ERROR,
	SERVER_AUTH_ERROR,
	SERVER_LIST_ERROR,
	SERVER_DELETEFILE_ERROR,
	SERVER_DELETEUSER_ERROR
};

#endif //SCREAMER_UTILLS_ERROR_CODES_H
