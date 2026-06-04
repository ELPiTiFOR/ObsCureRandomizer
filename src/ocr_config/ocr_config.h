#ifndef OCR_CONFIG_H
#define OCR_CONFIG_H

#include "config.h"

#define OCR_CONFIG_SUCCESS 0
#define OCR_CONFIG_FILE_NOT_FOUND 1
#define OCR_CONFIG_GAME_PATH_NOT_FOUND 2
#define OCR_CONFIG_ALREADY_SETUP 3

/*
**  It is expected that the `ocr.config` file defines a `game_path`
*/

extern config *ocr_config;

int setup_ocr_config(void);
char *get_ocr_config_error_msg(int status);

#endif /* !OCR_CONFIG_H */