#ifndef __TRADE_DEFINE_H__
#define __TRADE_DEFINE_H__



enum
{
	LIMITTYPE_OPENLIMIT  = 1,		///< 1:限价建仓
	LIMITTYPE_SL_CLOSE   = 3,		///< 3:限价止损平仓
	LIMITTYPE_TP_CLOSE   = 2,		///< 2:限价止盈平仓
};


enum
{
	OPENDIRECTOR_BUY    = 1,		///< 1:买
	OPENDIRECTOR_SELL   = 2,		///< 2:卖
};


enum
{
	INIT_MESSAGE_START  = 1,		///< 1:开始
	INIT_MESSAGE_END    = 2,		///< 2:结束
};


enum
{
	MARKET_STATUS_CLOSE = 0,		///< 0:闭市
	MARKET_STATUS_OPEN  = 1,		///< 1:开市
};


enum
{
	MONEY_OUT			= 1,		///< 1:出金
	MONEY_IN			= 2,		///< 2:入金
};

enum
{
	BANK_OF_CHINA		= 3,		///< 3:中国银行
};

enum
{
	DEFAULT_TRADERANGE	= 15,		///< 15:市价单允许点差的起始默认值
	MAX_TRADERANGE		= 16,		///< 16:市价单允许点差的最大值
	MIN_TRADERANGE		= 17,		///< 17:市价单允许点差的最小值
};

enum
{
	NOT_DISPLAY			=  0,		///< 不显示
	IS_DISPLAY			=  1,		///< 显示
};


enum
{
	OLD_COMMODITY		=  0,		///< 旧商品
	NEW_COMMODITY		=  1,		///< 新商品
};


#endif  //__TRADE_DEFINE_H__
