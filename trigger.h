#ifndef TRIGGER_H
#define TRIGGER_H

#define MAX_TRIGGERPOINTS 10

class Trigger{
	private:
		int mLen;
		int mWidth;
		int mHeight;
		int mValue;
		int mMidpoint;
		int *mTriggerpoints;
		unsigned short int *mData;
	public:
		Trigger();
		~Trigger();
		
		void setMid(int dataMidpoint);
	
		void setData(unsigned short int *data, int len);
		
		void setValue(int value);
		int getValue();
		int getPos(int triggerNr);
		int getPos();
};



#endif