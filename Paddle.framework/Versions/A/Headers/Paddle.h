//
//  Paddle.h
//  Paddle
//
//  Created by Paddle on 03/10/2017.
//  Introduced in v4.0.
//  Copyright © 2018 Paddle. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import "PADAlert.h"
#import "PADCheckoutOptions.h"
#import "PADDisplayConfiguration.h"
#import "PADError.h"
#import "PADProduct.h"
#import "PADProductConfiguration.h"

//! Project version number for Paddle.
FOUNDATION_EXPORT double PaddleVersionNumber;

//! Project version string for Paddle.
FOUNDATION_EXPORT const unsigned char PaddleVersionString[];

#pragma mark - Enums

/**
 * @discussion The following constants describe the possible states of the license activation
 * after the license (de)activation process.
 */
typedef NS_ENUM(NSInteger, PADActivationState) {
    /**
     * @brief The product was activated as part of the license activation process.
     */
    PADActivationActivated,

    /**
     * @brief The product was deactivated as part of the license deactivation process.
     */
    PADActivationDeactivated,

    /**
     * @brief The product (de)activation process was abandoned.
     */
    PADActivationAbandoned,

    /**
     * @brief The product (de)activation process has failed, possibly due to network connectivity issues
     * or an invalid license code.
     */
    PADActivationFailed,
};

/**
 * @brief The completion block called when an action was attempted on the activation dialog or the dialog was cancelled.
 * The block is given the state of the (de)activation attempt.
 */
typedef void (^PADActivationStatusCompletion)(PADActivationState activationState);

/**
 * @discussion The following constants describe the possible states of the checkout
 * after we've tried to show it to the user.
 */
typedef NS_ENUM(NSInteger, PADCheckoutState) {
    /**
     * @discussion The checkout was successful and the product was purchased.
     */
    PADCheckoutPurchased,

    /**
     * @discussion The user cancelled the checkout before the product was purchased.
     */
    PADCheckoutAbandoned,

    /**
     * @discussion The checkout failed to load or the order processing took too long to complete.
     */
    PADCheckoutFailed,

    /**
     * @brief The checkout was completed, but the transaction was flagged for manual processing.
     * @discussion The Paddle team will handle the transaction manually. If the order is approved,
     * the buyer will be able to activate the product later, when the approved order has been processed.
     */
    PADCheckoutFlagged
};

/**
 * @brief The completion block to be called when an action was attempted on the checkout dialog.
 * The block is given the state of the checkout attempt and data relevant to the checkout.
 */
typedef void (^PADCheckoutStateCompletion)(PADCheckoutState state, NSDictionary *_Nullable checkoutData);

/**
 * @discussion The following constants describe the possible UI dialogs we may display
 * to the user. Some UI dialogs play multiple roles, e.g. the license dialog handles activation,
 * deactivation and viewing the activation.
 */
typedef NS_ENUM(NSInteger, PADUIType) {
    /**
     * @discussion Product information UI
     */
    PADUIProduct,

    /**
     * @discussion License activation, deactivation or viewing existing activation UI
     */
    PADUILicense,

    /**
     * @discussion The checkout web container
     */
    PADUICheckout,

    /**
     * @discussion Other non-custom UI displayed by the SDK, such as NSAlerts
     */
    PADUIOther
};

/**
 * @discussion The following constants describe the possible actions a user can trigger
 * on the Paddle dialogs.
 */
typedef NS_ENUM(NSInteger, PADTriggeredUIType) {
    /**
     * @discussion Specifies that the user cancelled the checkout, activation, or that the SDK will show product information.
     */
    PADTriggeredUITypeShowProductAccess,

    /**
     * @discussion Specifies that the user chose to purchase the product.
     */
    PADTriggeredUITypeShowCheckout,

    /**
     * @discussion Specifies that the user chose to activate a license.
     */
    PADTriggeredUITypeShowActivate,

    /**
     * @discussion Specifies that the user chose to continue their trial.
     */
    PADTriggeredUITypeContinueTrial,

    /**
     * @discussion Specifies that the license was activated using the SDK UI.
     */
    PADTriggeredUITypeActivated,

    /**
     * @discussion Specifies that the license was deactivated using the SDK UI.
     */
    PADTriggeredUITypeDeactivated,

    /**
     * @discussion Specifies that the user chose to cancel an action.
     */
    PADTriggeredUITypeCancel,

    /**
     * @brief The previous UI action completed successfully and no further UI action needs to be taken by Paddle.
     * @discussion The finished UI type does not signal success or failure. Check the PADProduct properties for the
     * expected result, e.g. whether the product was activated.
     */
    PADTriggeredUITypeFinished
};

#pragma mark - Delegate Methods

/**
 * @discussion The Paddle delegate is called to configure the behavior of the Paddle SDK (mostly involving UI) and
 * handle errors that could not otherwise be handled. All protocol methods are optional and the
 * default behaviour is described in the method documentation.
 */
@protocol PaddleDelegate <NSObject>

@optional
/**
 * @discussion Delegate method called when the Paddle SDK is about to present some UI to the user.
 * Allowing you customize how/if this is displayed. By default the UI is displayed in a window.
 *
 * @discussion The delegate method is always dispatched on the main dispatch queue, as this method
 * typically requires access to UI elements. But the delegate method may not be called asynchronously.
 *
 * @param uiType A PADUIType enum containing which type of UI is requesting to be displayed
 *
 * @return A PADDisplayConfiguration object to determine how/if UI should be displayed
 */
- (nullable PADDisplayConfiguration *)willShowPaddleUIType:(PADUIType)uiType product:(nonnull PADProduct *)product;

/**
 * @discussion Delegate method called when the Paddle SDK has shown some UI and it has been dismissed.
 * Allowing you to customize what should happen next.
 *
 * @discussion The delegate method is always dispatched on the main dispatch queue, as this method
 * typically requires access to UI elements. But the delegate method may not be called asynchronously.
 *
 * @param uiType A PADUIType enum containing which type of UI has finished
 *
 * @param triggeredUIType A PADTriggeredUIType enum containing the action which was performed to dismiss the UI
 *
 * @param product A PADProduct object to determine which product the UI being dismissed relates to
 */
- (void)didDismissPaddleUIType:(PADUIType)uiType triggeredUIType:(PADTriggeredUIType)triggeredUIType product:(nonnull PADProduct *)product;

/**
 * @discussion Delegate method called when the Paddle SDK is about to present an alert to the user.
 * Allowing you to cancel the alert. If this method is not implemented, then the alert is allowed.
 *
 * @discussion The delegate method is always dispatched on the main dispatch queue, as this method
 * is called by the alert, which is a UI element. But the delegate method may not be called asynchronously.
 *
 * @param alert A PADAlert objective, containing the alert type, message and error if appropriate
 *
 * @return A BOOL value indicating if the alert should be displayed or not
 */
- (BOOL)willShowPaddleAlert:(nonnull PADAlert *)alert;

/**
 * @discussion Delegate method called when an error occurred and the error could not be handled by a more
 * relevant handler (e.g. a completion or action block).
 *
 * @discussion The delegate method is always dispatched on the main dispatch queue, but it may not be called
 * asynchronously.
 *
 * @param error An NSError object describing an error that occured in the Paddle SDK
 */
- (void)paddleDidError:(nonnull NSError *)error;

@end

@class PADProductWindowController;
@class PADCheckoutWindowController;
@class PADActivateWindowController;

#pragma mark - Properties
/**
 * @discussion The primary interface for the Paddle SDK, mostly used to present UI and toggle SDK-wide configuration.
 */
@interface Paddle : NSObject

/**
 * @discussion The Paddle delegate is called to configure the behavior of the Paddle SDK (mostly involving UI) and
 * handle errors that could not otherwise be handled. All protocol methods are optional and the
 * default behaviour is described in the method documentation.
 */
@property (nullable, weak) id<PaddleDelegate> delegate;

/**
 * @discussion Your SDK API Key obtained from an SDK product on your vendor dashboard
 */
@property (nonnull, copy, readonly) NSString *apiKey;

/**
 * @discussion Your Vendor ID for your Paddle account obtained from your vendor dashboard
 */
@property (nonnull, copy, readonly) NSString *vendorID;

/**
 * @discussion The product ID that was used to instantiate the Paddle instance.
 */
@property (nonnull, copy, readonly) NSString *productID;

/**
 * @brief On expiry of any product trial or for a product without a trial, if the user chooses to exit the app
 * rather than purchase the displayed product at this time, force-close the app from the product access dialog.
 * @discussion By default closing the app is left up to the \c applicationWillTerminate: method
 * of the app delegate. But by setting this property to \c YES we will exit the app's process with an exit code of 0.
 * This prevents the application from reacting to the app closing.
 */
@property BOOL canForceExit;

#pragma mark - Initialization & Access

/**
 * @discussion Initializes your Paddle Product with a configuration
 *
 * @param vendorID an NSString containing your Vendor ID for your Paddle account obtained from your vendor dashboard.
 * @param apiKey an NSString containing your SDK Product API Key obtained from your vendor dashboard.
 * @param productID an NSString containing your Paddle Product ID. Obtained from your vendor dashboard. This should be a SDK product, other product types can be worked with after this point.
 * @param configuration a PADProductConfiguration object, which can contain default information about your product such as price, name, etc, which is used on first run and no internet connection for UI.
 *
 * @return A Paddle shared instance object
 */
+ (nullable instancetype)sharedInstanceWithVendorID:(nonnull NSString *)vendorID
                                             apiKey:(nonnull NSString *)apiKey
                                          productID:(nonnull NSString *)productID
                                      configuration:(nullable PADProductConfiguration *)configuration;

/**
 * @discussion Used to get the shared instance object any time after initialization
 *
 * @return a Paddle shared instance object
 */
+ (nullable instancetype)sharedInstance;

#pragma mark - Debug

/**
 * @discussion Used to turn on debugging logging and helpers
 */
+ (void)enableDebug;

#pragma mark - UI
#pragma mark-- Access Control / Activation

/**
 * @discussion Show a Product Information Dialog, with options to start a checkout or enter a license code
 *
 * @param product A PADProduct object for the Paddle product you wish to be shown
 */
- (void)showProductAccessDialogWithProduct:(nonnull PADProduct *)product;

/**
 * @discussion Show UI for user to activate a license code.
 *
 * @param product A PADProduct object for the Paddle product you wish to activate a license for
 * @param email An optional email to prefill the email field of the activation dialog. If the product has been
 * activated, this parameter will be ignored.
 * @param licenseCode An optional license code to prefill the license code field of the activation dialog. If the
 * product has been activated, this parameter will be ignored.
 * @param activationStatusCompletion A completion block to be called when an action has been attempted on the activation dialog.
 * The completion block is called on the main dispatch queue.
 */
- (void)showLicenseActivationDialogForProduct:(nonnull PADProduct *)product
                                        email:(nullable NSString *)email
                                  licenseCode:(nullable NSString *)licenseCode
                   activationStatusCompletion:(nullable PADActivationStatusCompletion)activationStatusCompletion;

#pragma mark-- Checkout

/**
 * @brief Show a dialog for a user to purchase a Paddle product.
 *
 * @discussion The completion handler is passed the state of the checkout and, if available, relevant data.
 * The data is a dictionary with 2 top-level keys, both are optional and may be omitted: "checkout" and "order".
 * The "checkout" key, if included, is a dictionary with 2 keys: "checkout_id" and "email"; the checkout ID is
 * always included and is the ID of the checkout as is visible in the URL of a loaded checkout, and the email is
 * the buyer's email. The "order" key, if included, is a dictionary with the full response from Paddle's order
 * information API. Please refer to Paddle’s order information API for an example of the response. Note that the
 * "state" will always be "processed".
 *
 * @discussion Unlike the product access and license activation dialogs, the checkout dialog cannot be prevented
 * from showing. The reason for this is that the other dialogs can be replicated relatively easily, whereas the
 * checkout dialog is quite complex. Hence we do not recommend or enable creating a custom checkout dialog.
 *
 * @param product A PADProduct object for the Paddle product you wish to purchase.
 * @param options A PADCheckoutOptions object used to change the behaviour of the checkout.
 * @param checkoutStatusCompletion A completion block to be called when an action has been attempted on the checkout dialog.
 * The completion block is called on the main dispatch queue.
 */
- (void)showCheckoutForProduct:(nonnull PADProduct *)product
                       options:(nullable PADCheckoutOptions *)options
      checkoutStatusCompletion:(nullable PADCheckoutStateCompletion)checkoutStatusCompletion;

#pragma mark-- License Recovery

/**
 * @brief Recover licenses for an SDK product by emailing the user with their license codes.
 * @discussion The user's email must be collected previously to calling this method. No dialogs are shown
 * to collect the email or to inform the user of the result of the recovery.
 * @discussion If no completion handler is given, then the error is passed to the delegate. If no delegate
 * is set or the delegate does not respond to `paddleDidError:`, then the error is silently discarded.
 *
 * @param product A PADProduct object for the Paddle product you wish to recover licenses for. Only SDK products
 * are currently supported.
 * @param email An NSString containing the users email address
 * @param completion An optional completion block to be used when the license recovery process has finished.
 * This handler is executed on the main dispatch queue, but the handler may not be executed asynchronously.
 */
- (void)recoverLicenseForProduct:(nonnull PADProduct *)product
                           email:(nonnull NSString *)email
                      completion:(nullable void (^)(BOOL recoveryEmailSent, NSError *_Nullable error))completion;

/**
 * @brief Recover licenses for an SDK product by emailing the user with their license codes.
 * @discussion The user's email is collected through a dialog. The result of the recovery is displayed
 * to the user.
 * @discussion The user may be prompted to enter their email again if the recovery failed due to an
 * invalid email.
 * @discussion If no completion handler is given, then the error is passed to the delegate. If no delegate
 * is set or the delegate does not respond to `paddleDidError:`, then the error is silently discarded.
 * @discussion The user may abort the license recovery process. In this case the completion handler will report
 * no error and no recovery email sent.
 *
 * @param product A PADProduct object for the Paddle product you wish to recover licenses for. Only SDK products
 * are currently supported.
 * @param completion An optional completion block to be used when the license recovery process has finished.
 * This handler is executed on the main dispatch queue.
 */
- (void)showLicenseRecoveryForProduct:(nonnull PADProduct *)product
                           completion:(nullable void (^)(BOOL recoveryEmailSent, NSError *_Nullable error))completion;

#pragma mark-- Audience Subscribe Prompt

/**
 * @discussion Show email subscribe prompt to collect email and consent for Audience.
 *
 * @param message An NSString containing an optional custom message you would like to display to your user
 * @param companyName An NSString containing your company name, which will be displayed to your user
 * @param product The product which the user is interested in
 */
- (void)showEmailSubscribePromptWithMessage:(nullable NSString *)message
                                companyName:(nonnull NSString *)companyName
                                    product:(nonnull PADProduct *)product;

#pragma mark - Audience (silently)

/**
 * @discussion Directly add a user's email address to Audience.
 * @param email An NSString containing the email address you wish to subscribe
 * @param consent A BOOL indicating if the user has opted in to marketing emails
 * @param product The product which the user is interested in
 */
- (void)sendEmailSubscribe:(nonnull NSString *)email
                   consent:(BOOL)consent
                   product:(nonnull PADProduct *)product;

#pragma mark - Products

/**
 * @discussion Used to get a list of all products initialized for the app.
 *
 * @return products An NSArray of PADProduct objects
 */
- (nonnull NSArray *)allProducts;


@end