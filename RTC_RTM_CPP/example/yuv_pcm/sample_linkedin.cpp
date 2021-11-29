//  Usur's code
//
//  Created by 
//  Copyright (c) 
//

#include <csignal>
#include <cstring>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>

#include "AgoraRefCountedObject.h"
#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "common/log.h"
#include "common/opt_parser.h"
#include "common/sample_common.h"
#include "common/sample_connection_observer.h"
#include "common/sample_local_user_observer.h"

int linkedin_helloworld() {
    std::cout << "Hello World from Linkedin!\n";
    return 0;
}  
