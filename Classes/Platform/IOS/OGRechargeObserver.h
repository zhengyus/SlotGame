#import <Foundation/Foundation.h>
#import <StoreKit/SKPaymentQueue.h>
#import <StoreKit/SKProductsRequest.h>

@interface OGRechargeObserver : NSObject<SKPaymentTransactionObserver, SKProductsRequestDelegate>
{    
    SKPaymentTransaction* currentTransaction_; // 正在进行的交易
}
@property(nonatomic, retain) SKPaymentTransaction* currentTransaction;

+ (OGRechargeObserver*)sharedRechargeObserver;

// 验证产品列表
- (void)verifyProducts:(NSSet*)products;

// 购买产品
- (void)buyProduct:(NSString*)productID;

// 完成当前交易，从队列中删除当前交易
- (void)finishCurrentTransaction;

//处理未完成的交易
-(void)processPendingTransactions;

@end
