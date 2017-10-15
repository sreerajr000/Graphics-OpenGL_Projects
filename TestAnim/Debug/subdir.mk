################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Animation.cpp \
../AnimationChannel.cpp \
../AnimationController.cpp \
../Bone.cpp \
../Main.cpp \
../Mesh.cpp \
../Model.cpp 

OBJS += \
./Animation.o \
./AnimationChannel.o \
./AnimationController.o \
./Bone.o \
./Main.o \
./Mesh.o \
./Model.o 

CPP_DEPS += \
./Animation.d \
./AnimationChannel.d \
./AnimationController.d \
./Bone.d \
./Main.d \
./Mesh.d \
./Model.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


