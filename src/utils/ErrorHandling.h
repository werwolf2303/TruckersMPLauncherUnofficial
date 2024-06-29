//
// Created by werwolf2303 on 6/29/24.
//

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H



class ErrorHandling {
public:
    enum ErrorType {
        GENERIC_API, //Happened in the api
        DESCRIPTIVE_API, //Happened in the api (with Description)
        GENERIC, //Happened in code
        DESCRIPTIVE //Happened in code (with Description)
    };
};



#endif //ERRORHANDLING_H
