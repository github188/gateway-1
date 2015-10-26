#ifndef __TRADEAPI_COMMON_TRADECLIENT_H__
#define __TRADEAPI_COMMON_TRADECLIENT_H__



namespace TradeAPI
{
    namespace common
    {

        class Event;

        class TradeClient
        {
            public:

                /* The callback method for processing of received events */
                virtual void processEvent(const Event* pEvent) = 0;
                virtual ~TradeClient(){}
        };

    }
}



#endif //__TRADEAPI_COMMON_TRADECLIENT_H__


