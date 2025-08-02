# Expense & Savings Tracker - C++ Application

A comprehensive command-line interface (CLI) application for tracking personal expenses and savings with intelligent spending alerts.

## Features

### ✅ Core Functionality
- **User-friendly CLI interface** with menu-driven navigation
- **Manual transaction entry** for income and expenses
- **Excel/CSV file import** support for bulk data loading
- **Intelligent spending alerts** at 70%, 80%, and 90% of income
- **Automatic data persistence** with CSV storage
- **Category-wise financial analysis**
- **Data export functionality**

### 🚨 Smart Alert System
The application provides three levels of spending alerts:
- **70% Alert**: Monitor your spending carefully
- **80% Warning**: Consider reducing expenses  
- **90% Critical**: Immediate action required

## Requirements

### System Requirements
- C++17 compatible compiler (g++ recommended)
- Operating System: Linux, macOS, or Windows (with MinGW)
- Minimum 50MB free disk space

### Dependencies
- Standard C++ Library (no external dependencies required)
- File system access for data storage

## Installation

### Option 1: Manual Compilation
```bash
# Compile using g++
g++ expense_tracker.cpp

# Run the application
./expense_tracker
```

### Option 2: Windows (MinGW)
```cmd
# Compile using MinGW g++
g++ expense_tracker.cpp

# Run the application
expense_tracker.exe
```

## Usage Guide

### Starting the Application
When you run the application, you'll see a menu with the following options:

```
╔══════════════════════════════════════════════╗
║       ADVANCED EXPENSE & SAVINGS TRACKER     ║
╠══════════════════════════════════════════════╣
║ 1. Add Transaction Manually                  ║
║ 2. Load from Excel/CSV File                  ║
║ 3. View All Transactions                     ║
║ 4. View Category Analysis                    ║
║ 5. Check Spending Alerts & Analysis          ║
║ 6. Export Data to CSV                        ║
║ 7. Set Budget Limits                         ║
║ 8. Exit                                      ║
╚══════════════════════════════════════════════╝
```

### Adding Transactions Manually
1. Select option 1 from the main menu
2. Choose transaction type: `income` or `expense`
3. Enter the amount (must be positive)
4. Provide a description
5. Specify a category
6. Enter date (or press Enter for current date)

### Importing from Excel/CSV Files
1. Prepare your CSV file with the following format:
   ```
   Date,Description,Amount,Category,Type
   2024-01-01,Salary,5000.00,Job,income
   2024-01-02,Grocery Shopping,150.75,Food,expense
   ```
2. Select option 2 from the main menu
3. Enter the filename (with extension)
4. The application will process and import all valid transactions

### Sample CSV File
A sample CSV file (`sample_financial_data.csv`) is provided for testing:
- Contains sample income and expense transactions
- Demonstrates the correct format for importing data
- Can be used to test the import functionality

## File Structure

```
expense-tracker/
├── expense_tracker.cpp          # Main application source code
├── sample_financial_data.csv    # Sample data for testing
├── README.md                    # This documentation
├── financial_data.csv           # Auto-generated data file (created on first run)
└── exported_data.csv            # Example of exported data (user-created)
```

## Data Storage

### Automatic Data Persistence
- All transactions are automatically saved to `financial_data.csv`
- Data is loaded automatically when the application starts
- No manual save operation required

### Data Format
The application uses CSV format for data storage:
```csv
Date,Description,Amount,Category,Type
Mon Jan  1 10:30:15 2024,Salary,5000.00,Job,income
Tue Jan  2 14:20:30 2024,Grocery,150.75,Food,expense
```

## Alert System Details

The application calculates your expense-to-income ratio and provides alerts:

### Alert Levels
- **Below 70%**: ✅ GOOD! Your spending is under control
- **70-79%**: 🔔 ALERT! Monitor your spending carefully
- **80-89%**: ⚠️ WARNING! Consider reducing expenses
- **90%+**: 🚨 CRITICAL! Immediate action required

### Calculation
```
Expense Percentage = (Total Expenses / Total Income) × 100
```

## Advanced Features

### Category Analysis
- View spending breakdown by category
- Separate income and expense categorization
- Identify highest spending categories

### Export Functionality
- Export all data to CSV format
- Choose custom filename for exports
- Maintain data portability

### Error Handling
- Input validation for amounts and transaction types
- File I/O error handling
- Graceful handling of malformed CSV data

## Troubleshooting

### Common Issues

1. **Compilation Errors**
   - Ensure you have a C++17 compatible compiler
   - Check that all source files are in the same directory

2. **File Not Found Errors**
   - Verify the CSV file path when importing
   - Ensure the file has proper read permissions

3. **Data Not Persisting**
   - Check write permissions in the application directory
   - Ensure sufficient disk space

### Getting Help
If you encounter issues:
1. Check the error messages displayed by the application
2. Verify file permissions and paths
3. Ensure proper CSV format for imported files

## Development


### Code Structure
- **ExpenseTracker Class**: Main application logic
- **Transaction Struct**: Data structure for financial transactions  
- **File I/O Functions**: CSV reading/writing operations
- **Alert System**: Spending analysis and warning system

## License

This project is provided as-is for educational and personal use.

## Contributing

Feel free to submit issues, feature requests, or improvements to enhance the application.

---

**Happy Financial Tracking! 💰📊**
