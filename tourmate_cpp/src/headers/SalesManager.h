#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include "Sales.h"
#include "Vehicle.h"
#include "User.h"
#include "AuthenticationManager.h"
#include "VehicleManager.h"

/**
 * @class SalesManager
 * @brief Comprehensive sales and booking management system with transaction processing,
 *        payment handling, customer management, and revenue analytics
 * 
 * Features:
 * - Complete sales CRUD operations (Create, Read, Update, Delete)
 * - Advanced booking system with vehicle availability checking
 * - Multiple payment method support (Cash, Card, Digital Wallet)
 * - Customer management and booking history
 * - Revenue analytics and financial reporting
 * - Integration with Vehicle Management for real-time availability
 * - Role-based access control with audit logging
 * - Trip planning and route optimization
 * - Fare calculation engine with dynamic pricing
 * - Booking confirmation and receipt generation
 */
class SalesManager {
public:
    // Payment method enumeration
    enum class PaymentMethod {
        CASH,
        CREDIT_CARD,
        DEBIT_CARD,
        DIGITAL_WALLET,
        BANK_TRANSFER,
        UNKNOWN
    };
    
    // Booking status enumeration
    enum class BookingStatus {
        PENDING,
        CONFIRMED,
        IN_PROGRESS,
        COMPLETED,
        CANCELLED,
        REFUNDED,
        UNKNOWN
    };
    
    // Trip type enumeration
    enum class TripType {
        ONE_WAY,
        ROUND_TRIP,
        MULTI_STOP,
        CHARTER,
        UNKNOWN
    };

private:
    std::vector<std::shared_ptr<Sales>> salesRecords;           // Sales collection
    std::shared_ptr<AuthenticationManager> authManager;         // Authentication system
    std::shared_ptr<VehicleManager> vehicleManager;            // Vehicle management system
    std::string salesDataPath;                                 // Sales CSV file path
    std::string bookingDataPath;                               // Booking CSV file path
    std::string customerDataPath;                              // Customer CSV file path
    std::string auditLogPath;                                  // Audit log path
    bool dataLoaded;                                           // Data load status
    
    // Customer structure for booking management
    struct Customer {
        std::string customerId;
        std::string name;
        std::string email;
        std::string phone;
        std::string address;
        std::chrono::system_clock::time_point registrationDate;
        int totalBookings;
        double totalSpent;
        bool isVip;
    };
    
    // Booking structure for trip management
    struct Booking {
        std::string bookingId;
        std::string customerId;
        std::string vehicleId;
        std::string fromLocation;
        std::string toLocation;
        std::chrono::system_clock::time_point departureTime;
        std::chrono::system_clock::time_point arrivalTime;
        TripType tripType;
        int passengers;
        double basefare;
        double totalFare;
        double discount;
        PaymentMethod paymentMethod;
        BookingStatus status;
        std::string specialRequests;
        std::chrono::system_clock::time_point bookingDate;
        std::string bookedBy;
    };
    
    std::vector<Customer> customers;                           // Customer database
    std::vector<Booking> bookings;                            // Booking database
    
    // Internal utility methods
    bool hasPermission(const std::string& operation) const;
    void logOperation(const std::string& operation, const std::string& recordId = "", 
                     const std::string& details = "") const;
    std::string generateSalesId() const;
    std::string generateBookingId() const;
    std::string generateCustomerId() const;
    bool isValidSalesData(const Sales& sales, std::string& errorMessage) const;
    bool isValidBookingData(const Booking& booking, std::string& errorMessage) const;
    bool isValidCustomerData(const Customer& customer, std::string& errorMessage) const;
    
    // CSV utility methods
    std::vector<std::string> splitCSVLine(const std::string& line) const;
    std::string escapeCSVField(const std::string& field) const;
    std::string timePointToString(const std::chrono::system_clock::time_point& tp) const;
    std::chrono::system_clock::time_point stringToTimePoint(const std::string& str) const;
    
    // Fare calculation utilities
    double calculateBaseFare(const std::string& vehicleId, double distance) const;
    double calculateDiscounts(const Customer& customer, double baseFare) const;
    double calculateTaxes(double fare) const;
    
    // Vehicle availability checking
    bool isVehicleAvailable(const std::string& vehicleId, 
                           const std::chrono::system_clock::time_point& startTime,
                           const std::chrono::system_clock::time_point& endTime) const;

public:
    // Constructor and Destructor
    explicit SalesManager(std::shared_ptr<AuthenticationManager> authMgr,
                         std::shared_ptr<VehicleManager> vehicleMgr,
                         const std::string& salesDataFile = "data/sales.csv",
                         const std::string& bookingDataFile = "data/bookings.csv",
                         const std::string& customerDataFile = "data/customers.csv");
    ~SalesManager();

    // File Operations
    bool loadDataFromFiles();
    bool saveDataToFiles() const;
    bool createBackup() const;
    bool restoreFromBackup();
    bool exportSalesData(const std::string& filename) const;
    bool exportBookingData(const std::string& filename) const;
    bool importSalesData(const std::string& filename);
    bool importBookingData(const std::string& filename);

    // Sales CRUD Operations
    bool addSalesRecord(const Sales& sales);
    std::shared_ptr<Sales> getSalesRecord(const std::string& salesId) const;
    std::vector<std::shared_ptr<Sales>> getAllSalesRecords() const;
    bool updateSalesRecord(const std::string& salesId, const Sales& updatedSales);
    bool deleteSalesRecord(const std::string& salesId);

    // Customer Management
    bool addCustomer(const Customer& customer);
    Customer* getCustomer(const std::string& customerId);
    std::vector<Customer> getAllCustomers() const;
    bool updateCustomer(const std::string& customerId, const Customer& updatedCustomer);
    bool deleteCustomer(const std::string& customerId);
    std::vector<Customer> searchCustomers(const std::string& searchTerm) const;
    Customer* findCustomerByEmail(const std::string& email);
    Customer* findCustomerByPhone(const std::string& phone);

    // Booking Management
    std::string createBooking(const std::string& customerId, const std::string& vehicleId,
                             const std::string& fromLocation, const std::string& toLocation,
                             const std::chrono::system_clock::time_point& departureTime,
                             int passengers, TripType tripType = TripType::ONE_WAY,
                             const std::string& specialRequests = "");
    
    Booking* getBooking(const std::string& bookingId);
    std::vector<Booking> getAllBookings() const;
    std::vector<Booking> getCustomerBookings(const std::string& customerId) const;
    std::vector<Booking> getVehicleBookings(const std::string& vehicleId) const;
    bool updateBookingStatus(const std::string& bookingId, BookingStatus newStatus);
    bool cancelBooking(const std::string& bookingId, const std::string& reason = "");
    bool confirmBooking(const std::string& bookingId, PaymentMethod paymentMethod);

    // Trip Management
    bool startTrip(const std::string& bookingId);
    bool completeTrip(const std::string& bookingId, double actualDistance = -1.0);
    double calculateTripDuration(const Booking& booking) const;
    std::string generateTripReceipt(const std::string& bookingId) const;

    // Search and Filter Operations
    std::vector<std::shared_ptr<Sales>> searchSales(const std::string& searchTerm) const;
    std::vector<std::shared_ptr<Sales>> filterSalesByDateRange(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;
    std::vector<std::shared_ptr<Sales>> filterSalesByVehicle(const std::string& vehicleId) const;
    std::vector<std::shared_ptr<Sales>> filterSalesByCustomer(const std::string& customerId) const;
    std::vector<std::shared_ptr<Sales>> filterSalesByAmountRange(double minAmount, double maxAmount) const;
    
    std::vector<Booking> filterBookingsByStatus(BookingStatus status) const;
    std::vector<Booking> filterBookingsByDateRange(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;
    std::vector<Booking> filterBookingsByRoute(const std::string& route) const;

    // Advanced Search with Multiple Criteria
    struct SalesSearchCriteria {
        std::string searchTerm;
        std::chrono::system_clock::time_point startDate;
        std::chrono::system_clock::time_point endDate;
        std::string vehicleId;
        std::string customerId;
        double minAmount = -1.0;
        double maxAmount = -1.0;
        PaymentMethod paymentMethod = PaymentMethod::UNKNOWN;
        std::string sortBy = "date";
        bool ascending = false;
    };
    
    std::vector<std::shared_ptr<Sales>> advancedSalesSearch(const SalesSearchCriteria& criteria) const;

    // Payment Processing
    bool processPayment(const std::string& bookingId, PaymentMethod method, 
                       double amount, const std::string& paymentDetails = "");
    bool refundPayment(const std::string& bookingId, double refundAmount, 
                      const std::string& reason = "");
    std::vector<std::string> getSupportedPaymentMethods() const;
    bool validatePaymentMethod(PaymentMethod method) const;

    // Revenue Analytics and Reporting
    struct RevenueReport {
        double totalRevenue = 0.0;
        double totalRefunds = 0.0;
        double netRevenue = 0.0;
        int totalTransactions = 0;
        int totalBookings = 0;
        int completedTrips = 0;
        int cancelledBookings = 0;
        double averageTransactionValue = 0.0;
        double averageTripDuration = 0.0;
        std::map<PaymentMethod, double> revenueByPaymentMethod;
        std::map<std::string, double> revenueByVehicle;
        std::map<std::string, int> bookingsByRoute;
        std::string topVehicle;
        std::string topRoute;
        std::string topCustomer;
    };
    
    RevenueReport generateRevenueReport(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;
    
    RevenueReport getDailyReport() const;
    RevenueReport getWeeklyReport() const;
    RevenueReport getMonthlyReport() const;
    RevenueReport getYearlyReport() const;

    // Customer Analytics
    struct CustomerAnalytics {
        int totalCustomers = 0;
        int activeCustomers = 0;
        int vipCustomers = 0;
        double averageCustomerValue = 0.0;
        double customerRetentionRate = 0.0;
        std::map<std::string, int> customersByLocation;
        std::vector<Customer> topCustomers;
    };
    
    CustomerAnalytics getCustomerAnalytics() const;
    std::vector<Customer> getTopCustomers(int count = 10) const;
    double getCustomerLifetimeValue(const std::string& customerId) const;

    // Fleet Utilization Analytics
    struct FleetUtilization {
        std::map<std::string, double> vehicleUtilizationRate;
        std::map<std::string, double> vehicleRevenue;
        std::map<std::string, int> vehicleTrips;
        std::string mostUtilizedVehicle;
        std::string leastUtilizedVehicle;
        double averageUtilization = 0.0;
    };
    
    FleetUtilization getFleetUtilization(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;

    // Data Validation and Integrity
    bool validateSalesData() const;
    bool validateBookingData() const;
    bool validateCustomerData() const;
    std::vector<std::string> findInconsistentData() const;
    int repairDataInconsistencies();

    // Utility Methods
    int getSalesCount() const;
    int getBookingCount() const;
    int getCustomerCount() const;
    bool salesRecordExists(const std::string& salesId) const;
    bool bookingExists(const std::string& bookingId) const;
    bool customerExists(const std::string& customerId) const;
    std::string getNextAvailableSalesId() const;
    std::string getNextAvailableBookingId() const;
    std::string getNextAvailableCustomerId() const;

    // Authentication Integration
    bool canCreateSales() const;
    bool canEditSales() const;
    bool canDeleteSales() const;
    bool canViewSales() const;
    bool canProcessRefunds() const;
    bool canViewReports() const;
    std::string getCurrentUserRole() const;
    std::string getCurrentUsername() const;

    // Audit and Logging
    std::vector<std::string> getAuditLog() const;
    bool clearAuditLog();
    void printAuditSummary() const;

    // Display Methods
    void displaySalesRecord(const std::shared_ptr<Sales>& sales) const;
    void displaySalesRecordList(const std::vector<std::shared_ptr<Sales>>& salesList) const;
    void displayBooking(const Booking& booking) const;
    void displayBookingList(const std::vector<Booking>& bookingList) const;
    void displayCustomer(const Customer& customer) const;
    void displayCustomerList(const std::vector<Customer>& customerList) const;
    void displayRevenueReport(const RevenueReport& report) const;
    void displayCustomerAnalytics(const CustomerAnalytics& analytics) const;
    void displayFleetUtilization(const FleetUtilization& utilization) const;

    // Helper Methods for Enums
    static std::string paymentMethodToString(PaymentMethod method);
    static PaymentMethod stringToPaymentMethod(const std::string& str);
    static std::string bookingStatusToString(BookingStatus status);
    static BookingStatus stringToBookingStatus(const std::string& str);
    static std::string tripTypeToString(TripType type);
    static TripType stringToTripType(const std::string& str);

    // Error Handling
    enum class SalesError {
        NONE,
        INVALID_DATA,
        DUPLICATE_ID,
        NOT_FOUND,
        PERMISSION_DENIED,
        FILE_ERROR,
        AUTHENTICATION_REQUIRED,
        VEHICLE_UNAVAILABLE,
        PAYMENT_FAILED,
        BOOKING_CONFLICT
    };
    
    SalesError getLastError() const { return lastError; }
    std::string getLastErrorMessage() const { return lastErrorMessage; }

private:
    mutable SalesError lastError = SalesError::NONE;
    mutable std::string lastErrorMessage;
    
    void setError(SalesError error, const std::string& message) const;
    void clearError() const;
};

#endif // SALESMANAGER_H