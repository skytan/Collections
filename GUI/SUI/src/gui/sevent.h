#ifndef SEVENT_H
#define SEVENT_H


#define EVENT_TYPE_KEY 1
#define EVENT_TYPE_QUIT 2
#define EVENT_TYPE_TIMER 3
#define EVENT_TYPE_TEXTCHANGE 4
#define EVENT_TYPE_UPDATE 5
#define EVENT_TYPE_CLICK 6


class sEvent
{
public:
	sEvent(sEvent *e = 0);
	virtual ~sEvent();

	virtual class sEvent* select();
	
	void beginSelect();
	void endSelect();
	int getValue() const;
	int getType() const;
	int getMask() const;
	class sFramebuffer* getFramebuffer();
	
      void setType(const class sEvent *e);
      void setValue(const class sEvent *e);
	void setValue(int value);
	void setType(int type);
	void setMask(int mask);
	void setFramebuffer(class sFramebuffer* pfb);

protected:
	int type;
	int value;
	int mask;

	bool busy;
};

#endif
