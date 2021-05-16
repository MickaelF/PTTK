#pragma once

#define DELETE_COPY_CONSTR_ASSIGN(classname) \
    classname(const classname&) = delete; \
    classname(classname&&) = delete; \
    classname& operator=(const classname&) = delete; \
    classname& operator=(classname&&) = delete; 
