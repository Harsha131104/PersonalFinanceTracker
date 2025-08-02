#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <filesystem>
#include <ctime>
#include <numeric>

using namespace std;

// Structure to represent a financial transaction
struct Transaction {
    string date;
    string description;
    double amount;
    string category;
    string type; // "income" or "expense"

    Transaction(const string& d, const string& desc, double amt, const string& cat, const string& t)
        : date(d), description(desc), amount(amt), category(cat), type(t) {}
};

// Structure for budget limits
struct BudgetLimit {
    string category;
    double monthlyLimit;

    BudgetLimit(const string& cat, double limit) : category(cat), monthlyLimit(limit) {}
};

// Enhanced ExpenseTracker class with budget management
class ExpenseTracker {
private:
    vector<Transaction> transactions;
    vector<BudgetLimit> budgetLimits;
    double totalIncome;
    double totalExpenses;
    string dataFile;
    string budgetFile;

public:
    ExpenseTracker() : totalIncome(0.0), totalExpenses(0.0), 
                      dataFile("financial_data.csv"), budgetFile("budget_limits.csv") {
        loadData();
        loadBudgetLimits();
    }

    ~ExpenseTracker() {
        saveData();
        saveBudgetLimits();
    }

    // Get current date as string
    string getCurrentDate() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string dateStr(dt);
        dateStr.pop_back(); // Remove newline
        return dateStr;
    }

    // Get current month-year string
    string getCurrentMonthYear() {
        time_t now = time(0);
        struct tm* timeinfo = localtime(&now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m", timeinfo);
        return string(buffer);
    }

    // Validate amount input
    bool isValidAmount(double amount) {
        return amount > 0;
    }

    // Calculate expense percentage of income
    double getExpensePercentage() {
        if (totalIncome == 0) return 0;
        return (totalExpenses / totalIncome) * 100;
    }

    // Get monthly expenses by category
    map<string, double> getMonthlyExpensesByCategory(const string& monthYear) {
        map<string, double> monthlyExpenses;

        for (const auto& transaction : transactions) {
            if (transaction.type == "expense" && transaction.date.find(monthYear) != string::npos) {
                monthlyExpenses[transaction.category] += transaction.amount;
            }
        }

        return monthlyExpenses;
    }

    // Check budget limits for current month
    void checkBudgetLimits() {
        if (budgetLimits.empty()) {
            cout << "\n📊 No budget limits set. Use option 8 to set budget limits." << endl;
            return;
        }

        string currentMonth = getCurrentMonthYear();
        map<string, double> monthlyExpenses = getMonthlyExpensesByCategory(currentMonth);

        cout << "\n=== BUDGET ANALYSIS FOR " << currentMonth << " ===" << endl;
        cout << left << setw(20) << "Category" << setw(12) << "Spent" 
             << setw(12) << "Budget" << setw(12) << "Remaining" << "Status" << endl;
        cout << string(70, '-') << endl;

        bool anyOverBudget = false;

        for (const auto& budget : budgetLimits) {
            double spent = monthlyExpenses[budget.category];
            double remaining = budget.monthlyLimit - spent;
            double percentage = (spent / budget.monthlyLimit) * 100;

            cout << left << setw(20) << budget.category
                 << "$" << right << setw(10) << fixed << setprecision(2) << spent
                 << "$" << right << setw(10) << fixed << setprecision(2) << budget.monthlyLimit
                 << "$" << right << setw(10) << fixed << setprecision(2) << remaining;

            if (percentage >= 100) {
                cout << " 🚨 OVER BUDGET!";
                anyOverBudget = true;
            } else if (percentage >= 80) {
                cout << " ⚠️  WARNING!";
            } else if (percentage >= 60) {
                cout << " 🔔 ALERT";
            } else {
                cout << " ✅ GOOD";
            }
            cout << endl;
        }

        if (anyOverBudget) {
            cout << "\n🚨 BUDGET ALERT: You have exceeded budget limits in some categories!" << endl;
        }
    }

    // Enhanced alert system with budget integration
    void checkAndShowAlerts() {
        double percentage = getExpensePercentage();

        cout << "\n=== COMPREHENSIVE SPENDING ANALYSIS ===" << endl;
        cout << "Total Income: $" << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Available Balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << endl;
        cout << "Expense Percentage: " << fixed << setprecision(1) << percentage << "%" << endl;

        // Overall spending alerts
        if (percentage >= 90) {
            cout << "\n🚨 CRITICAL WARNING! 🚨" << endl;
            cout << "You are spending " << fixed << setprecision(1) << percentage << "% of your income!" << endl;
            cout << "IMMEDIATE ACTION REQUIRED: Cut down expenses drastically!" << endl;
            cout << "Recommended actions:" << endl;
            cout << "• Review all non-essential expenses" << endl;
            cout << "• Cancel unnecessary subscriptions" << endl;
            cout << "• Consider additional income sources" << endl;
        }
        else if (percentage >= 80) {
            cout << "\n⚠️  WARNING! ⚠️" << endl;
            cout << "You are spending " << fixed << setprecision(1) << percentage << "% of your income!" << endl;
            cout << "Consider reducing your expenses to maintain financial stability." << endl;
            cout << "Recommended actions:" << endl;
            cout << "• Set budget limits for categories" << endl;
            cout << "• Track daily expenses more carefully" << endl;
        }
        else if (percentage >= 70) {
            cout << "\n🔔 ALERT! 🔔" << endl;
            cout << "You are spending " << fixed << setprecision(1) << percentage << "% of your income!" << endl;
            cout << "Monitor your spending carefully to avoid financial strain." << endl;
            cout << "Consider setting budget limits for better control." << endl;
        }
        else {
            cout << "\n✅ EXCELLENT! Your spending is under control." << endl;
            cout << "Keep up the good financial habits!" << endl;
        }

        // Budget-specific alerts
        checkBudgetLimits();

        // Savings recommendation
        double recommendedSavings = totalIncome * 0.20; // 20% savings goal
        double actualSavings = totalIncome - totalExpenses;

        cout << "\n💰 SAVINGS ANALYSIS:" << endl;
        cout << "Recommended Savings (20%): $" << fixed << setprecision(2) << recommendedSavings << endl;
        cout << "Current Savings: $" << fixed << setprecision(2) << actualSavings << endl;

        if (actualSavings >= recommendedSavings) {
            cout << "🎉 Great job! You're meeting your savings goal!" << endl;
        } else {
            cout << "💡 Try to save $" << fixed << setprecision(2) 
                 << (recommendedSavings - actualSavings) << " more to reach 20% savings rate." << endl;
        }
    }

    // Set budget limits for categories
    void setBudgetLimit() {
        string category;
        double limit;

        cout << "\n=== SET BUDGET LIMIT ===" << endl;

        // Show existing budget limits
        if (!budgetLimits.empty()) {
            cout << "Existing budget limits:" << endl;
            for (const auto& budget : budgetLimits) {
                cout << "• " << budget.category << ": $" << fixed << setprecision(2) 
                     << budget.monthlyLimit << " per month" << endl;
            }
            cout << endl;
        }

        cout << "Enter category name: ";
        cin.ignore();
        getline(cin, category);

        cout << "Enter monthly budget limit: $";
        cin >> limit;

        if (!isValidAmount(limit)) {
            cout << "Invalid amount! Please enter a positive number." << endl;
            return;
        }

        // Check if category already exists
        auto it = find_if(budgetLimits.begin(), budgetLimits.end(),
                         [&category](const BudgetLimit& budget) {
                             return budget.category == category;
                         });

        if (it != budgetLimits.end()) {
            cout << "Budget limit for '" << category << "' updated from $" 
                 << fixed << setprecision(2) << it->monthlyLimit 
                 << " to $" << limit << endl;
            it->monthlyLimit = limit;
        } else {
            budgetLimits.push_back(BudgetLimit(category, limit));
            cout << "Budget limit set for '" << category << "': $" 
                 << fixed << setprecision(2) << limit << " per month" << endl;
        }
    }

    // Load budget limits from file
    void loadBudgetLimits() {
        ifstream file(budgetFile);
        if (!file.is_open()) {
            return; // No budget file exists yet
        }

        string line;
        // Skip header line
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string category, limitStr;

            getline(ss, category, ',');
            getline(ss, limitStr, ',');

            if (!limitStr.empty()) {
                double limit = stod(limitStr);
                budgetLimits.push_back(BudgetLimit(category, limit));
            }
        }

        file.close();
    }

    // Save budget limits to file
    void saveBudgetLimits() {
        if (budgetLimits.empty()) return;

        ofstream file(budgetFile);
        if (!file.is_open()) return;

        // Write header
        file << "Category,MonthlyLimit\n";

        // Write budget limits
        for (const auto& budget : budgetLimits) {
            file << budget.category << "," << fixed << setprecision(2) 
                 << budget.monthlyLimit << "\n";
        }

        file.close();
    }

    // Add manual transaction with enhanced feedback
    void addTransaction() {
        string type, description, category, dateInput;
        double amount;

        cout << "\n=== ADD TRANSACTION ===" << endl;
        cout << "Enter transaction type (income/expense): ";
        cin >> type;
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type != "income" && type != "expense") {
            cout << "Invalid transaction type! Please enter 'income' or 'expense'." << endl;
            return;
        }

        cout << "Enter amount: $";
        cin >> amount;

        if (!isValidAmount(amount)) {
            cout << "Invalid amount! Please enter a positive number." << endl;
            return;
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter description: ";
        getline(cin, description);

        cout << "Enter category: ";
        getline(cin, category);

        cout << "Enter date (or press Enter for current date): ";
        getline(cin, dateInput);

        if (dateInput.empty()) {
            dateInput = getCurrentDate();
        }

        // Add transaction
        transactions.push_back(Transaction(dateInput, description, amount, category, type));

        // Update totals
        if (type == "income") {
            totalIncome += amount;
            cout << "✅ Income of $" << fixed << setprecision(2) << amount << " added successfully!" << endl;
        } else {
            totalExpenses += amount;
            cout << "💸 Expense of $" << fixed << setprecision(2) << amount << " recorded successfully!" << endl;

            // Check if this expense affects any budget limits
            if (!budgetLimits.empty()) {
                string currentMonth = getCurrentMonthYear();
                map<string, double> monthlyExpenses = getMonthlyExpensesByCategory(currentMonth);
                double categorySpent = monthlyExpenses[category];

                auto budgetIt = find_if(budgetLimits.begin(), budgetLimits.end(),
                                      [&category](const BudgetLimit& budget) {
                                          return budget.category == category;
                                      });

                if (budgetIt != budgetLimits.end()) {
                    double percentage = (categorySpent / budgetIt->monthlyLimit) * 100;
                    cout << "📊 Category '" << category << "' spending: $" << fixed << setprecision(2) 
                         << categorySpent << " / $" << budgetIt->monthlyLimit 
                         << " (" << fixed << setprecision(1) << percentage << "%)" << endl;

                    if (percentage >= 100) {
                        cout << "🚨 WARNING: Budget exceeded for '" << category << "'!" << endl;
                    } else if (percentage >= 80) {
                        cout << "⚠️ Approaching budget limit for '" << category << "'!" << endl;
                    }
                }
            }
        }

        // Show quick financial summary
        cout << "\n📈 Quick Summary:" << endl;
        cout << "Total Income: $" << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Available Balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << endl;

        checkAndShowAlerts();
    }

    // Load data from CSV file
    void loadData() {
        ifstream file(dataFile);
        if (!file.is_open()) {
            cout << "No existing data file found. Starting fresh." << endl;
            return;
        }

        string line;
        // Skip header line
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string date, description, category, type, amountStr;

            getline(ss, date, ',');
            getline(ss, description, ',');
            getline(ss, amountStr, ',');
            getline(ss, category, ',');
            getline(ss, type, ',');

            if (!amountStr.empty()) {
                double amount = stod(amountStr);
                transactions.push_back(Transaction(date, description, amount, category, type));

                if (type == "income") {
                    totalIncome += amount;
                } else {
                    totalExpenses += amount;
                }
            }
        }

        file.close();
        cout << "Loaded " << transactions.size() << " transactions from file." << endl;
    }

    // Save data to CSV file
    void saveData() {
        ofstream file(dataFile);
        if (!file.is_open()) {
            cout << "Error: Could not save data to file." << endl;
            return;
        }

        // Write header
        file << "Date,Description,Amount,Category,Type\n";

        // Write transactions
        for (const auto& transaction : transactions) {
            file << transaction.date << ","
                 << transaction.description << ","
                 << fixed << setprecision(2) << transaction.amount << ","
                 << transaction.category << ","
                 << transaction.type << "\n";
        }

        file.close();
    }

    // Load data from Excel/CSV file provided by user
    void loadFromExcelFile() {
        string filename;
        cout << "\n=== LOAD FROM EXCEL/CSV FILE ===" << endl;
        cout << "Enter the filename (with extension): ";
        cin >> filename;

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file '" << filename << "'" << endl;
            return;
        }

        string line;
        int lineCount = 0;
        int addedCount = 0;

        cout << "Expected CSV format: Date,Description,Amount,Category,Type" << endl;
        cout << "Processing file..." << endl;

        // Skip header line
        getline(file, line);
        lineCount++;

        while (getline(file, line)) {
            lineCount++;
            stringstream ss(line);
            string date, description, category, type, amountStr;

            try {
                getline(ss, date, ',');
                getline(ss, description, ',');
                getline(ss, amountStr, ',');
                getline(ss, category, ',');
                getline(ss, type, ',');

                // Clean up the strings (remove quotes if present)
                date.erase(remove(date.begin(), date.end(), '"'), date.end());
                description.erase(remove(description.begin(), description.end(), '"'), description.end());
                category.erase(remove(category.begin(), category.end(), '"'), category.end());
                type.erase(remove(type.begin(), type.end(), '"'), type.end());

                transform(type.begin(), type.end(), type.begin(), ::tolower);

                if (!amountStr.empty() && (type == "income" || type == "expense")) {
                    double amount = stod(amountStr);
                    if (amount > 0) {
                        transactions.push_back(Transaction(date, description, amount, category, type));

                        if (type == "income") {
                            totalIncome += amount;
                        } else {
                            totalExpenses += amount;
                        }
                        addedCount++;
                    }
                }
            } catch (const exception& e) {
                cout << "Error processing line " << lineCount << ": " << e.what() << endl;
            }
        }

        file.close();
        cout << "Successfully loaded " << addedCount << " transactions from '" << filename << "'" << endl;
        checkAndShowAlerts();
    }

    // Display all transactions with enhanced formatting
    void displayTransactions() {
        if (transactions.empty()) {
            cout << "\nNo transactions found." << endl;
            return;
        }

        cout << "\n=== ALL TRANSACTIONS (" << transactions.size() << " total) ===" << endl;
        cout << left << setw(20) << "Date" << setw(30) << "Description" 
             << setw(12) << "Amount" << setw(15) << "Category" << setw(10) << "Type" << endl;
        cout << string(87, '-') << endl;

        for (const auto& transaction : transactions) {
            cout << left << setw(20) << transaction.date.substr(0, 19)
                 << setw(30) << transaction.description.substr(0, 29)
                 << "$" << right << setw(10) << fixed << setprecision(2) << transaction.amount
                 << left << setw(15) << transaction.category.substr(0, 14)
                 << setw(10) << transaction.type << endl;
        }

        checkAndShowAlerts();
    }

    // Display enhanced category summary with insights
    void displayCategorySummary() {
        if (transactions.empty()) {
            cout << "\nNo transactions found." << endl;
            return;
        }

        map<string, double> incomeByCategory;
        map<string, double> expensesByCategory;

        for (const auto& transaction : transactions) {
            if (transaction.type == "income") {
                incomeByCategory[transaction.category] += transaction.amount;
            } else {
                expensesByCategory[transaction.category] += transaction.amount;
            }
        }

        cout << "\n=== COMPREHENSIVE CATEGORY ANALYSIS ===" << endl;

        if (!incomeByCategory.empty()) {
            cout << "\n💰 INCOME BY CATEGORY:" << endl;
            cout << left << setw(25) << "Category" << setw(15) << "Amount" << "Percentage" << endl;
            cout << string(50, '-') << endl;
            for (const auto& pair : incomeByCategory) {
                double percentage = (pair.second / totalIncome) * 100;
                cout << left << setw(25) << pair.first
                     << "$" << right << setw(13) << fixed << setprecision(2) << pair.second
                     << "  " << fixed << setprecision(1) << percentage << "%" << endl;
            }
        }

        if (!expensesByCategory.empty()) {
            cout << "\n💸 EXPENSES BY CATEGORY:" << endl;
            cout << left << setw(25) << "Category" << setw(15) << "Amount" << "Percentage" << endl;
            cout << string(50, '-') << endl;

            // Sort expenses by amount (highest first)
            vector<pair<string, double>> sortedExpenses(expensesByCategory.begin(), expensesByCategory.end());
            sort(sortedExpenses.begin(), sortedExpenses.end(),
                 [](const pair<string, double>& a, const pair<string, double>& b) {
                     return a.second > b.second;
                 });

            for (const auto& pair : sortedExpenses) {
                double percentage = (pair.second / totalExpenses) * 100;
                cout << left << setw(25) << pair.first
                     << "$" << right << setw(13) << fixed << setprecision(2) << pair.second
                     << "  " << fixed << setprecision(1) << percentage << "%";

                // Highlight top spending categories
                if (percentage >= 30) {
                    cout << " 🔥 HIGH";
                } else if (percentage >= 15) {
                    cout << " ⚠️  MEDIUM";
                }
                cout << endl;
            }

            // Show top spending insight
            if (!sortedExpenses.empty()) {
                cout << "\n💡 INSIGHT: '" << sortedExpenses[0].first 
                     << "' is your highest expense category ($" 
                     << fixed << setprecision(2) << sortedExpenses[0].second << ")" << endl;
            }
        }

        checkAndShowAlerts();
    }

    // Export data to CSV with enhanced options
    void exportToCSV() {
        string filename;
        cout << "\nEnter filename for export (with .csv extension): ";
        cin >> filename;

        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create export file." << endl;
            return;
        }

        // Write header with additional metadata
        file << "Date,Description,Amount,Category,Type\n";
        file << "# Export generated on: " << getCurrentDate() << "\n";
        file << "# Total transactions: " << transactions.size() << "\n";
        file << "# Total income: $" << fixed << setprecision(2) << totalIncome << "\n";
        file << "# Total expenses: $" << fixed << setprecision(2) << totalExpenses << "\n";
        file << "# Net balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << "\n";

        // Write transactions
        for (const auto& transaction : transactions) {
            file << transaction.date << ","
                 << transaction.description << ","
                 << fixed << setprecision(2) << transaction.amount << ","
                 << transaction.category << ","
                 << transaction.type << "\n";
        }

        file.close();
        cout << "✅ Data exported successfully to '" << filename << "'" << endl;
        cout << "📊 Export includes " << transactions.size() << " transactions and summary metadata." << endl;
    }
};

// Display enhanced main menu
void displayMenu() {
    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║       ADVANCED EXPENSE & SAVINGS TRACKER     ║" << endl;
    cout << "╠══════════════════════════════════════════════╣" << endl;
    cout << "║ 1. Add Transaction Manually                  ║" << endl;
    cout << "║ 2. Load from Excel/CSV File                  ║" << endl;
    cout << "║ 3. View All Transactions                     ║" << endl;
    cout << "║ 4. View Category Analysis                    ║" << endl;
    cout << "║ 5. Check Spending Alerts & Analysis          ║" << endl;
    cout << "║ 6. Export Data to CSV                        ║" << endl;
    cout << "║ 7. Set Budget Limits                         ║" << endl;    
    cout << "║ 8. Exit                                      ║" << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;
    cout << "Enter your choice (1-8): ";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    ExpenseTracker tracker;
    int choice;

    cout << "🎉 Welcome to Advanced Expense & Savings Tracker!" << endl;
    cout << "💰 Your complete solution for financial management with intelligent alerts!" << endl;
    cout << "📊 Features: Budget limits, spending analysis, and comprehensive reporting." << endl;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                tracker.addTransaction();
                break;
            case 2:
                tracker.loadFromExcelFile();
                break;
            case 3:
                tracker.displayTransactions();
                break;
            case 4:
                tracker.displayCategorySummary();
                break;
            case 5:
                tracker.checkAndShowAlerts();
                break;
            case 6:
                tracker.exportToCSV();
                break;
            case 7:
                tracker.setBudgetLimit();
                break;
            case 8:
                cout << "\n💝 Thank you for using Advanced Expense & Savings Tracker!" << endl;
                cout << "💾 Your data has been saved automatically." << endl;
                cout << "🚀 Keep up the great financial management!" << endl;
                return 0;
            default:
                cout << "❌ Invalid choice! Please enter a number between 1 and 8." << endl;
        }

        cout << "\n⏎ Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}