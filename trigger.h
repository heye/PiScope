#ifndef TRIGGER_H
#define TRIGGER_H


class Trigger{
	private:
		int mLen;
		int mWidth;
		int mHeight;
		int mValue;
		int mMidpoint;
		unsigned short int *mData;
	public:
		void setMid(int dataMidpoint);
	
		void setData(unsigned short int *data, int len);
		
		void setValue(int value);
		int getValue();
		int getPos(int value);
		int getPos();
};



#endif