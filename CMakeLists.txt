cmake_minimum_required (VERSION 3.26)

# PROJECT NAME 
project ("FantasyEngine")

# SETTING CMAKE OPTIONS 
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)

# SETTING OUTPUT DIRECTORY 
add_subdirectory("Source")