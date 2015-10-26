#ifndef __TRADE_STRUCT_H__
#define __TRADE_STRUCT_H__


#define  MAX_CUSTOMERNAME_LEN             128
#define  MAX_COMMODITYNAME_LEN            128
#define  MAX_COMMODITYCODE_LEN            16
#define  MAX_COMMODITYTYPE_LEN            16
#define  MAX_LOGINACCONT_LEN              32
#define  MAX_MESSAGE_LEN                  512

#define  MAX_BULLETIN_HEADER_LEN          128
#define  MAX_BULLETIN_CONTENT_LEN         4000
#define  MAX_PUBLISHERNAME_LEN            64

#define  MAX_BANKPWD_LEN				  128
#define  MAX_FUNDPWD_LEN				  128
#define  MAX_BANKACCNT_LEN				  32
#define  MAX_BANKNAME_LEN				  64
#define  MAX_CERTNUMBER_LEN               32
#define  MAX_SPECIFICATIONUNIT_LEN        16

/// 登录用户信息结构体
typedef struct _USER_INFO
{
	int  LoginID;		///< 登录ID
	char LoginName[MAX_LOGINACCONT_LEN];	///< 用户名
	int  UserID;		///< 用户ID
	int  MemberID;		///< 会员ID
	int  CustomerID;	///< 客户ID
	int  UserType;		///< 用户类型
	char CustomerName[MAX_COMMODITYNAME_LEN];	///< 客户名称
}UserInfo;


/// 实时行情结构体
typedef struct _REALTIEM_QUOTE
{
    int       CommodityID;	///< 商品ID
    double    SellPrice;	///< 卖出价
    double    BuyPrice;		///< 买入价
    double    HighPrice;	///< 最高价
    double    LowPrice;		///< 最低价
    long long QuoteTime;	///< 报价时间
}RealTimeQuote;



/// 账户信息结构体
typedef struct _ACCOUNT_INFO
{
    char   Account[MAX_LOGINACCONT_LEN];	///< 账户名
    char   LoginAccount[MAX_LOGINACCONT_LEN];	///< 登录名
	char   CustomerName[MAX_CUSTOMERNAME_LEN];	///< 客户名称
	double NAVPrice;            ///< 净值
    double Amount;              ///< 余额
	double OpenProfit;          ///< 浮动盈亏
	double ExchangeReserve;     ///< 交易准备金
	double PerformanceReserve;  ///< 履约准备金
	double FrozenReserve;       ///< 冻结准备金
	double RiskRate;            ///< 风险率
}AccountInfo;


/// 客户银行信息
typedef struct _CUSTOMERBANKINFO
{
	int		BankID;								///< 银行ID
	char	BankAccount[MAX_BANKACCNT_LEN];		///< 银行卡号
	char	BankName[MAX_BANKNAME_LEN];			///< 银行名称
	char    FundPwd[MAX_FUNDPWD_LEN];			///< 资金密码
	int		SignStatus;							///< 签约状态
	int		CertificateType;					///< 证件类型
	char	CertificateNum[MAX_CERTNUMBER_LEN];	///< 证件号码
}CustomerBankInfo;


/// 交易所余额信息
typedef struct _BOURSEMONEY_INFO
{
	double Amount;				///< 交易所余额
	double AmountAvailable;		///< 交易所可用资金
	double AmountFetchable;		///< 交易所可提取资金
}BourseMoneyInfo;


/// 商品信息结构体
typedef struct _COMMODITY_INFO
{
    int       CommodityID;	///< 商品ID
    char      CommodityName[MAX_COMMODITYNAME_LEN];		///< 商品名称
	long long CommodityRight;	///< 商品权限
	long long TradeRight;	///< 交易权限
	double    AgreeUnit;	///< 合约单位
	int       Currency;		///< 货币种类
	double    MinQuoteChangeUnit;	///< 最小行情变动单位
	double    MinPriceUnit;	///< 最小价格单位
	double    FixedSpread;	///< 固定点差
    double    BuyPrice;		///< 买入价
    double    SellPrice;	///< 卖出价
    double    HighPrice;	///< 最高价
    double    LowPrice;		///< 最低价
    long long QuoteTime;	///< 报价时间
	int       CommodityClass;   ///< 商品类型
	char      CommodityClassName[MAX_COMMODITYNAME_LEN];	///< 商品类型名称
	int       CommodityMode;	///< 处理方式 0-旧商品 1-新商品
	int       IsDisplay;    ///< 是否展示
	int       CommodityGroupID;	///< 商品组ID
	char      CommodityGroupName[MAX_COMMODITYNAME_LEN];	///< 商品组名称
	double    WeightStep;	///< 重量步进值
	double    WeightRadio;	///< 重量换算
    int       TradeType;
    int       SpecificationRate;
    char      SpecificationUnit[MAX_SPECIFICATIONUNIT_LEN];
}CommodityInfo;


/// 持仓单信息结构体
typedef struct _HOLDPOSITION_INFO
{
    long long HoldPositionID;	///< 持仓单ID
	int       CommodityID;		///< 商品ID
	char      CommodityName[MAX_COMMODITYNAME_LEN];	///< 商品名称
	int       OpenType;			///< 建仓类型  --- ::1:客户下单  --- 4:限价下单
	int       OpenDirector;		///< 建仓方向
	int       Quantity;			///< 持仓数量
	double	  TotalWeight;		///< 持仓总重量(KG)
	double    OpenPrice;		///< 建仓价格
	double    HoldPositionPrice;	///< 持仓价
	double    ClosePrice;		///< 平仓价
	long long SLLimitOrderID;	///< 止损单ID
	double    SLPrice;			///< 止损价
	long long TPLimitOrderID;	///< 止盈单ID
	double    TPPrice;			///< 止盈价
	double    OpenProfit;		///< 浮动盈亏
	double    CommissionAmount;	///< 手续费
	long long OpenDate;			///< 建仓时间
	double    AgreeMargin;		///< 履约保证金
	double    Freezemargin;		///< 冻结保证金
	double    OverdueFindFund;	///< 滞纳金
}HoldPositionInfo;


/// 限价单信息结构体
typedef struct _LIMITORDER_INFO
{
	long long LimitOrderID;		///< 限价单ID
	int       CommodityID;		///< 商品ID
	char      CommodityName[MAX_COMMODITYNAME_LEN];		///< 商品名称
	int       LimitType;		///< 限价单类型	--- ::1:限价建仓 ---2:止盈平仓  ---3:止损平仓
	int       OrderType;		///< 建仓类型  --- ::1.客户下单
	int       OpenDirector;		///< 建仓方向
	double    OrderPrice;		///< 建仓价
	double    SLPrice;			///< 止损价
	double    TPPrice;			///< 止盈价
	int       OpenQuantity;		///< 持仓数量
	double	  TotalWeight;		///< 持仓总重量(KG)
	long long CreateDate;		///< 建仓时间
	long long ExpireType;		///< 失效类型
	long long UpdateDate;		///< 更新时间
	double    FreeszMargin;		///< 冻结保证金

}LimitOrderInfo;


/// 平仓单信息结构体
typedef struct _CLOSEPOSITION_INFO
{
	long long ClosePositionID;	///< 平仓单ID
	int       CommodityID;		///< 商品ID
	char      CommodityName[MAX_COMMODITYNAME_LEN];	///< 商品名称
	int       CloseDirector;	///< 平仓方向
	double    OpenPrice;		///< 建仓价
	double    HoldPrice;		///< 持仓价
	double    ClosePrice;		///< 平仓价
	int       Quantity;			///< 持仓数量
	double	  TotalWeight;		///< 持仓总重量(KG)
	long long OpenPositionID;	///< 建仓单ID
	double    CommissionAmount;	///< 手续费
	long long OpenDate;			///< 建仓时间
	long long CloseDate;		///< 平仓时间
	int       MemberID;			///< 会员ID
	int       OpenType;			///< 建仓类型  --- ::1:客户下单  --- 4:限价下单
	int       CloseType;		///< 平仓类型  --- ::1:客户下单  --- 4:限价下单  --- 5:斩仓强平
}ClosePositionInfo;


/// 持仓汇总单信息结构体
typedef struct _HOLDPOSITION_TOTAL_INFO
{
    int    CommodityID;			///< 商品ID
	char   CommodityName[MAX_COMMODITYNAME_LEN];	///<  商品名称
	int    OpenDirector;		///< 建仓方向
	int    Quantity;			///< 持仓数量
	double TotalWeight;			///< 持仓总重量(KG)
	double OpenPriceTotal;		///< 建仓总值
	double AvgOpenPrice;		///< 建仓均价
	double HoldPriceTotal;		///< 持仓总值
	double AvgHoldPrice;		///< 持仓均价
	double ClosePrice;			///< 平仓价
	double OpenProfit;			///< 浮动盈亏

}HoldPositionTotalInfo;


/// 限价撤销单信息结构体
typedef struct _LIMITREVOKE_INFO
{
	long long LimitOrderID;		///< 限价单ID
	int       LimitType;		///< 限价单类型
}LimitRevokeInfo;


/// 客户端配置信息结构体
typedef struct _CLIENTCONFIG
{
	int     TradeMode;			///< 交易模式
	int     MarketParamType;	///< 参数类型
	double  MarketParamValue;	///< 参数取值
}ClientConfig;


/// 商品费项信息结构体
typedef struct _COMMODITYFEEGROUP
{
	int CommodityID;			///< 商品标识
	int CommodityFeeID;			///< 费用标识
	int ChargeType;				///< 计提方式
	int CollectType;			///< 收取方式
	int FeeType;				///< 费用父类
	int FeeSubType;				///< 费用子类
	double  Value;				///< 取值
	int EffectStatus;			///< 生效状态
	int IsDelivery;				///< 是否交割费用
}CommodityFeeGroup;



///  市价单建仓配置信息结构体
typedef struct _OPENMARKETORDER_CONF
{
	int       CommodityID;		///< 商品ID
    double    MinQuantity;		///< 每单最大交易手数
    double    MaxQuantity;		///< 每单最小交易手数
	double    MinTradeRange;	///< 市价单允许点差的最小值
	double    MaxTradeRange;	///< 市价单允许点差的最大值
	double    DefaultTradeRange;	///< 市价单允许点差的起始默认值
	double    AgreeUnit;		///< 合约单位
	int       CommodityMode;	///< 处理方式 0-旧商品 1-新商品
	double    WeightStep;		///< 重量步进值
	double    WeightRadio;		///< 重量换算
	double    MinTotalWeight;	///< 每单最小交易总重
	double	  MaxTotalWeight;	///< 每单最大交易总重
	double    DepositeRate;		///< 准备金率
	double    SpecificationRate;	///< 规格系数
}OpenMarketOrderConf;



/// 限价单建仓配置信息结构体
typedef struct _OPENLIMITORDER_CONF
{
    int       CommodityID;		///< 商品ID
    double    MinQuantity;		///< 每单最小交易手数
    double    MaxQuantity;		///< 每单最大交易手数
	double    LimitSpread;		///< 限价点差
	double    FixSpread;		///< 固定点差
	double    TPSpread;			///< 止盈点差
	double    SLSpread;			///< 止损点差
	double    MinPriceUnit;		///< 最小价格单位
	double    AgreeUnit;		///< 合约单位
	int       CommodityMode;	///< 处理方式 0-旧商品 1-新商品
	double    WeightStep;		///< 重量步进值
	double    WeightRadio;		///< 重量换算
	double    MinTotalWeight;   ///< 每单最小交易总重
	double    MaxTotalWeight;   ///< 每单最大交易总重
	double    DepositeRate;     ///< 准备金率
	double    SpecificationRate;    ///< 规格系数
}OpenLimitOrderConf;

/// 市价单平仓配置信息结构体
typedef struct _CLOSEMARKETORDER_CONF
{
	int       CommodityID;		///< 商品ID
	double    MinQuantity;		///< 每单最小交易手数
	double    MaxQuantity;		///< 每单最大交易手数
	double    MinTradeRange;	///< 市价单允许点差的最小值
	double    MaxTradeRange;	///< 市价单允许点差的最大值
	double    DefaultTradeRange;	///< 市价单允许点差的起始默认值
	double    AgreeUnit;		///< 合约单位
	int       CommodityMode;	///< 处理方式 0-旧商品 1-新商品
	double    WeightStep;		///< 重量步进值
	double    WeightRadio;		///< 重量换算
	double    MinTotalWeight;   ///< 每单最小交易总重
	double    MaxTotalWeight;   ///< 每单最大交易总重
}CloseMarketOrderConf;


/// 限价单平仓配置信息结构体
typedef struct _LIMITCLOSEPOSITION_CONF
{
	int       CommodityID;		///< 商品ID
	double    FixedSpread;		///< 固定点差
	double    TPSpread;			///< 止盈点差
	double    SLSpread;			///< 止损点差
	double    MinPriceUnit;		///< 最小价格单位
	double    AgreeUnit;		///< 合约单位
	int       CommodityMode;	///< 处理方式 0-旧商品 1-新商品
	double    WeightRadio;		///< 重量换算
}LimitClosePositionConf;


/// 处理结果结构体
typedef struct _PROCESSRESULT
{
	int  RetCode;				///< 返回码
	char Message[MAX_MESSAGE_LEN];		///< 返回信息
}ProcessResult;


/// 公告结构体
typedef struct _SYSBULLETIN_INFO
{
	int       LoginID;		///< 登录ID
	long long BulletinID;   ///< 公告ID
	int       TradeMode;		///< 交易模式
	int       BulletinType;		///< 公告类型
	int       BulletinPriorty;  ///< 优先级
	char      BulletinHeader[MAX_BULLETIN_HEADER_LEN];		///< 公告消息头
	char      BulletinContent[MAX_BULLETIN_CONTENT_LEN];		///< 公告消息正文
	char      PublisherName[MAX_PUBLISHERNAME_LEN];		///< 发布方名
	int       BulletinMethod;		///< 公告方式
}SysBulletinInfo;

/// 市价单建仓 
typedef struct _OPENMARKETORDER_PARAM
{
	int nCommodityID;		///< 商品ID
	int nOpenDirector;		///< 建仓方向		--- ::OPENDIRECTOR_BUY:买  --- OPENDIRECTOR_SELL:卖
	double dbPrice;			///< 建仓单价
	double dbWeight;		///< 交易重量(kg)
	int nQuantity;			///< 交易数量
	double dbTradeRange;	///< 最大点差
	//int nOrderType;			///< 下单类型		--- 1:客户下单
}OpenMarketOrderParam;



/// 市价平仓单入参结构体 
typedef struct _CLOSEMARKETORDER_PARAM
{
	long long nHoldPositionID;		///< 持仓ID
	int nCommodityID;				///< 商品ID
	double dbWeight;				///< 交易重量(kg)
	int nQuantity;					///< 平仓数量
	int nTradeRange;				///< 最大点差
	double dbPrice;					///< 平仓价格
	//int nClosePositionType;			///< 平仓类型		--- 1:客户下单
}CloseMarketOrderParam;



/// 限价单建仓入参结构体 
typedef struct _OPENLIMITODER_PARAM
{
	int nCommodityID;			///< 商品ID
	int nExpireType;			///< 过期类型		--- 1:当日有效
	int nOpenDirector;			///< 建仓方向		--- ::OPENDIRECTOR_BUY:买  --- OPENDIRECTOR_SELL:卖
	double dbWeight;			///< 交易重量(kg)
	int nQuantity;				///< 建仓数量
	//int nOrderType;				///< 下单类型		--- 1:客户下单
	double dbOrderPrice;		///< 建仓单价
	double dbTPPrice;			///< 止盈价格
	double dbSLPrice;			///< 止损价格
}OpenLimitOrderParam;


/// 限价单平仓入参结构体
typedef struct _CLOSELIMITORDER_PARAM
{
	int nCommodityID;			///< 商品ID
	double dbClosePrice;		///< 平仓单价
	int nExpireType;			///< 过期类型		--- 1:当日有效
	long long nHoldPositionID;	///< 持仓ID
	//int nOrderType;				///< 下单类型		--- 1:客户下单
	double dbSLPrice;			///< 止盈价格
	double dbTPPrice;			///< 止损价格
}CloseLimitOrderParam;


/// 限价单撤销入参结构体
typedef struct _LIMITREVOKE_PARAM
{
	long long nLimitOrderID;	///< 限价单ID
	int nCommodityID;			///< 商品ID
	//int nOrderType;				///< 下单类型		--- 1:客户下单
	int nLimitType;				///< 限价单类型		--- ::LIMITTYPE_OPENLIMIT:建仓  --- LIMITTYPE_TP_CLOSE:止盈  --- LIMITTYPE_SL_CLOSE:止损
}LimitRevokeParam;



/// 批量平仓入参结构体 
typedef struct _CLOSEMARKETORDERMANY_PARAM
{
	int nCommodityID;			///< 商品ID
	double dbWeight;			///< 交易重量(kg)
	int nQuantity;				///< 平仓数量
	int nTradeRange;			///< 最大点差
	double dbPrice;				///< 平仓价格
	//int nClosePositionType;		///< 平仓类型		--- 1:一般平仓
	int nCloseDirector;			///< 平仓方向		--- ::OPENDIRECTOR_BUY:买  --- OPENDIRECTOR_SELL:卖
}CloseMarketOrderManyParam;


typedef struct	_MONEYINOUT_PARAM
{
	int OperateType;			///< 出入金类型		--- ::MONEY_OUT:出金  --- MONEY_IN:入金
	int Currency;				///< 币种		--- 1:人民币
	double Amount;				///< 出入金数量
	char FundPsw[MAX_FUNDPWD_LEN];	///< 资金密码
	char BankPsw[MAX_BANKPWD_LEN];	///< 银行密码
}MoneyInOutParam;


#endif //__STRUCT_H__
