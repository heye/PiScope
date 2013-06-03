#include "trigger.h"

Trigger::Trigger(){
	mTriggerpoints = new int[10];
}

Trigger::~Trigger(){
	delete mTriggerpoints;
}

void Trigger::setMid(int dataMidpoint){
	mMidpoint = dataMidpoint;
}

void Trigger::setData(unsigned short int *data, int len){
	mData = data;
	mLen = len;
}

void Trigger::setValue(int value){
	mValue = value;
}

int Trigger::getValue(){
	return mValue;
}

int Trigger::getPos(int value){
	setValue(value);
	return getPos();
}

int Trigger::getPos(){
	int found = 0;
	int i=1;
	while(found==0 && i+1 < mLen){
		if((mData[i-1]-mMidpoint)<mValue && (mData[i]-mMidpoint)<mValue 
			&& (mData[i+1]-mMidpoint)>mValue && (mData[i+2]-mMidpoint)>mValue){
			found = 1;			
			return i;
		}
		i++;
	}
	return -1;
}

/*
int Trigger::getPos(){
	int found = 0;
	int i=1;
	//int mTriggerpoints[10];
	int count=0;
	while(i+1 < mLen && count < MAX_TRIGGERPOINTS){
		if((mData[i-1]-mMidpoint)<mValue && (mData[i]-mMidpoint)<mValue 
			&& (mData[i+1]-mMidpoint)>mValue && (mData[i+2]-mMidpoint)>mValue){	
			
			mTriggerpoints[count] = i;
			count ++;
		}
		i++;
	}
	if(count > 0)
	return mTriggerpoints[5];
	return -1;
}*/
