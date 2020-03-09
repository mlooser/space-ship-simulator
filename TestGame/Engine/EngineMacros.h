#pragma once

#define PREVENT_COPY(class_name) class_name(const class_name&) = delete;\
                                 class_name& operator=(const class_name&) = delete

#define PREVENT_MOVE(class_name) class_name(class_name&&) = delete;\
                                 class_name& operator=(class_name&&) = delete


#define PREVENT_COPY_AND_MOVE(class_name)	PREVENT_COPY(class_name); \
											PREVENT_MOVE(class_name)