# Smart Personal Finance Tracker with Intelligent Budget Alerts

A hybrid C++/Web application demonstrating **7 data structures** for financial management with budget tracking and intelligent alerts. This project is designed for **Data Structures & Applications Lab (Part B)** evaluation.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Technology Stack](#technology-stack)
3. [Data Structures Implemented](#data-structures-implemented)
4. [Project Architecture](#project-architecture)
5. [Features](#features)
6. [Setup & Installation](#setup--installation)
7. [Running the Application](#running-the-application)
8. [API Reference](#api-reference)
9. [Frontend Pages](#frontend-pages)
10. [C++ Engine Details](#c-engine-details)
11. [File Structure](#file-structure)
12. [Demo Data](#demo-data)
13. [Time Complexity Analysis](#time-complexity-analysis)
14. [Viva Questions & Answers](#viva-questions--answers)

---

## Project Overview

This Smart Personal Finance Tracker is a full-stack application that combines:

- **C++ Backend Engine**: Implements 7 data structures for efficient financial data processing
- **FastAPI (Python)**: REST API server that bridges the frontend with the C++ engine
- **React Frontend**: Modern, responsive UI with Tailwind CSS and shadcn/ui components

The application allows users to:
- Track income and expenses
- Set and monitor category budgets with intelligent alerts (50%, 80%, 100% thresholds)
- Manage upcoming bills with a FIFO queue system
- View analytics and top spending patterns
- Undo operations using a stack-based system
- Get autocomplete suggestions using Trie

---

## Technology Stack

| Layer | Technology | Purpose |
|-------|-----------|---------|
| **Frontend** | React 19 | UI Framework |
| | Tailwind CSS | Styling |
| | shadcn/ui | Component Library |
| | React Router v7 | Navigation |
| | Axios | API Communication |
| | Sonner | Toast Notifications |
| | date-fns | Date Manipulation |
| **Backend API** | FastAPI | REST API Server |
| | Python 3.x | Backend Language |
| | Pydantic | Data Validation |
| **DSA Engine** | C++17 | Data Structure Implementation |
| | g++ | Compiler |
| **Data Storage** | JSON Files | Persistent Storage |

---

## Data Structures Implemented

### 1. Hash Map (`hashmap.h`)
- **Purpose**: Store category → budget mapping and expense totals
- **Operations**: 
  - `insert O(1)` average
  - `search O(1)` average
  - `update O(1)` average
  - `remove O(1)` average
- **Used In**: Budget management, Category expense tracking

### 2. Doubly Linked List (`linkedlist.h`)
- **Purpose**: Maintain transaction history with bidirectional traversal
- **Operations**:
  - `addFront O(1)`
  - `addBack O(1)`
  - `delete O(n)`
  - `traverseForward O(n)`
  - `traverseBackward O(n)`
- **Used In**: Transaction history, Recent transactions

### 3. Binary Search Tree - BST (`bst.h`)
- **Purpose**: Store transactions sorted by date for efficient range queries
- **Operations**:
  - `insert O(log n)` average
  - `rangeQuery O(log n + k)` where k is result count
  - `inorderTraversal O(n)`
- **Used In**: Date-wise queries, Monthly summaries

### 4. Max Heap (`heap.h`)
- **Purpose**: Identify top spending transactions and categories
- **Operations**:
  - `insert O(log n)`
  - `extractMax O(log n)`
  - `buildHeap O(n)`
  - `getTopK O(k log n)`
- **Used In**: Top expenses, Top categories analytics

### 5. Queue - FIFO (`queue.h`)
- **Purpose**: Manage upcoming bill payments in order
- **Operations**:
  - `enqueue O(1)`
  - `dequeue O(1)`
  - `peek O(1)`
- **Used In**: Bill management, Payment scheduling

### 6. Stack - LIFO (`stack.h`)
- **Purpose**: Implement undo functionality and recent transactions
- **Operations**:
  - `push O(1)`
  - `pop O(1)`
  - `peek O(1)`
- **Used In**: Undo operations, Recent transactions display

### 7. Trie (`trie.h`)
- **Purpose**: Category and payee autocomplete functionality
- **Operations**:
  - `insert O(m)` where m is word length
  - `prefixSearch O(m + k)` where k is results
- **Used In**: Category suggestions, Search autocomplete

---

## Project Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        FRONTEND (React)                         │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌──────────┐  │
│  │Dashboard│ │Transact.│ │ Budgets │ │  Bills  │ │Analytics │  │
│  └────┬────┘ └────┬────┘ └────┬────┘ └────┬────┘ └────┬─────┘  │
└───────┼──────────┼──────────┼──────────┼──────────┼──────────┘
        │          │          │          │          │
        └──────────┴──────────┴────┬─────┴──────────┘
                                   │ HTTP/JSON
                                   ▼
┌─────────────────────────────────────────────────────────────────┐
│                     BACKEND API (FastAPI)                       │
│                                                                 │
│  /api/dashboard     /api/transactions    /api/budgets           │
│  /api/bills         /api/analytics       /api/categories        │
└────────────────────────────┬────────────────────────────────────┘
                             │ subprocess + JSON
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                    C++ DSA ENGINE                               │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐               │
│  │ HashMap │ │LinkedLst│ │   BST   │ │  Heap   │               │
│  └─────────┘ └─────────┘ └─────────┘ └─────────┘               │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐                           │
│  │  Queue  │ │  Stack  │ │  Trie   │                           │
│  └─────────┘ └─────────┘ └─────────┘                           │
└────────────────────────────┬────────────────────────────────────┘
                             │ File I/O
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                    JSON DATA FILES                              │
│  transactions.json  budgets.json  bills.json  undo_stack.json   │
└─────────────────────────────────────────────────────────────────┘
```

---

## Features

### Dashboard
- Real-time balance, income, and expense overview
- Budget alerts at 50%, 80%, and 100% thresholds
- Recent transactions (Stack - LIFO)
- Top expenses (Max Heap)

### Transaction Management
- Add income/expense transactions
- Category autocomplete (Trie)
- Date-based filtering (BST range query)
- Search and filter transactions
- Delete transactions with undo support

### Budget Management
- Set spending limits by category (HashMap)
- Visual progress bars for each budget
- Color-coded alerts:
  - Green (< 50%): Normal
  - Yellow (50-79%): Caution
  - Orange (80-99%): Warning
  - Red (≥ 100%): Exceeded

### Bill Management
- Add upcoming bills to queue (FIFO)
- Visual queue representation
- Mark bills as paid
- Overdue bill highlighting

### Analytics
- Monthly summary with BST range queries
- Top 10 expenses (Max Heap extraction)
- Top spending categories (Category Max Heap)
- Custom date range reports

### Undo Functionality
- Undo last action using Stack
- Supports undo for:
  - Add/Delete transactions
  - Budget updates
  - Bill operations

### DSA Reference Page
- Complete documentation of all data structures
- Time complexity analysis
- 10 viva preparation questions with answers

---

## Setup & Installation

### Prerequisites
- **Node.js** >= 18.x
- **Python** >= 3.9
- **g++** (GNU C++ Compiler with C++17 support)
- **Yarn** package manager

### Step 1: Clone and Navigate
```bash
cd /app
```

### Step 2: Compile C++ Engine
```bash
cd backend/cpp
make clean
make
# or manually:
# g++ -std=c++17 -Wall -Wextra -O2 -o finance_engine main.cpp
cd ../..
```

This creates `finance_engine` (or `finance_engine.exe` on Windows) executable.

### Step 3: Install Backend Dependencies
```bash
cd backend
pip install -r requirements.txt
cd ..
```

### Step 4: Install Frontend Dependencies
```bash
cd frontend
yarn install
cd ..
```

---

## Running the Application

### Using Supervisor (Recommended)
The application is configured to run with supervisor:
```bash
# Start all services
sudo supervisorctl start all

# Restart services
sudo supervisorctl restart all

# Check status
sudo supervisorctl status
```

### Manual Startup

#### Start Backend Server
```bash
cd backend
uvicorn server:app --host 0.0.0.0 --port 8001 --reload
```

#### Start Frontend Development Server
```bash
cd frontend
yarn start
```

### Access the Application
- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:8001/api

---

## API Reference

### Base URL
All API endpoints are prefixed with `/api`

### Health Check
```
GET /api/health
```
Returns engine availability status.

### Dashboard
```
GET /api/dashboard
```
Returns balance, totals, counts, and undo availability.

**Response:**
```json
{
  "balance": 2570.00,
  "totalIncome": 5750.00,
  "totalExpenses": 3180.00,
  "transactionCount": 15,
  "budgetCount": 6,
  "billCount": 4,
  "canUndo": true
}
```

### Transactions

#### Get All Transactions
```
GET /api/transactions
```
Returns all transactions sorted by date (BST traversal).

#### Add Transaction
```
POST /api/transactions
Content-Type: application/json

{
  "type": "expense",
  "amount": 100.00,
  "category": "Food",
  "description": "Lunch",
  "date": "2025-07-15"
}
```

#### Get Recent Transactions
```
GET /api/transactions/recent?count=10
```
Returns recent transactions using Stack (LIFO).

#### Get Transactions by Date Range
```
GET /api/transactions/range?start_date=2025-07-01&end_date=2025-07-31
```
Uses BST range query.

#### Delete Transaction
```
DELETE /api/transactions/{transaction_id}
```

### Budgets

#### Get All Budgets
```
GET /api/budgets
```
Returns all budgets with spent amounts and alert levels (HashMap).

#### Set Budget
```
POST /api/budgets
Content-Type: application/json

{
  "category": "Food",
  "limit": 500.00
}
```

#### Get Budget Alerts
```
GET /api/budgets/alerts
```
Returns budgets that have exceeded 50% threshold.

### Bills

#### Get All Bills
```
GET /api/bills
```
Returns all bills in queue order (FIFO).

#### Add Bill
```
POST /api/bills
Content-Type: application/json

{
  "name": "Internet Bill",
  "amount": 75.00,
  "dueDate": "2025-07-20",
  "category": "Utilities"
}
```

#### Pay Bill
```
POST /api/bills/{bill_id}/pay
```

#### Delete Bill
```
DELETE /api/bills/{bill_id}
```

### Analytics

#### Get Top Expenses
```
GET /api/top-expenses?count=5
```
Uses Max Heap extraction.

#### Get Top Categories
```
GET /api/top-categories?count=5
```
Uses Category Max Heap.

#### Get Monthly Summary
```
GET /api/monthly-summary?month=2025-07
```
Uses BST month range query.

### Category Autocomplete

#### Get Suggestions
```
GET /api/categories/suggest?prefix=Foo
```
Uses Trie prefix search.

#### Get All Categories
```
GET /api/categories
```

### Undo
```
POST /api/undo
```
Undoes the last action using Stack pop.

### DSA Information
```
GET /api/dsa-info
```
Returns documentation about all data structures used.

---

## Frontend Pages

### 1. Dashboard (`/dashboard`)
- **File**: `/frontend/src/pages/Dashboard.jsx`
- **Features**: KPI cards, budget alerts, recent transactions, top expenses
- **Data Structures Shown**: Stack, Max Heap

### 2. Add Transaction (`/add-transaction`)
- **File**: `/frontend/src/pages/AddTransaction.jsx`
- **Features**: Transaction form with Trie autocomplete
- **Data Structures Shown**: Linked List, BST, Trie

### 3. Transactions (`/transactions`)
- **File**: `/frontend/src/pages/Transactions.jsx`
- **Features**: Transaction list, filters, date range queries
- **Data Structures Shown**: BST, Doubly Linked List

### 4. Budgets (`/budgets`)
- **File**: `/frontend/src/pages/Budgets.jsx`
- **Features**: Budget cards with progress bars, alert thresholds
- **Data Structures Shown**: HashMap

### 5. Bills (`/bills`)
- **File**: `/frontend/src/pages/Bills.jsx`
- **Features**: Bill queue visualization, pay/delete actions
- **Data Structures Shown**: Queue (FIFO)

### 6. Analytics (`/analytics`)
- **File**: `/frontend/src/pages/Analytics.jsx`
- **Features**: Monthly summary, date range reports, top charts
- **Data Structures Shown**: BST, Max Heap

### 7. DSA Reference (`/dsa-info`)
- **File**: `/frontend/src/pages/DSAInfo.jsx`
- **Features**: Data structure documentation, viva questions

---

## C++ Engine Details

### Compilation
```bash
cd backend/cpp
make          # Compile with optimizations
make debug    # Compile with debug symbols
make clean    # Remove compiled binary
```

### Input/Output Format
The C++ engine communicates via JSON through stdin/stdout:

**Input:**
```json
{
  "command": "add_transaction",
  "params": {
    "type": "expense",
    "amount": 100.00,
    "category": "Food",
    "description": "Lunch",
    "date": "2025-07-15"
  }
}
```

**Output:**
```json
{
  "success": true,
  "transaction": {
    "id": "txn_1234567890_1",
    "type": "expense",
    "amount": 100.00,
    "category": "Food",
    "description": "Lunch",
    "date": "2025-07-15"
  },
  "canUndo": true
}
```

### Supported Commands
| Command | Description |
|---------|-------------|
| `add_transaction` | Add a new transaction |
| `delete_transaction` | Delete transaction by ID |
| `get_transactions` | Get all transactions (BST traversal) |
| `get_recent_transactions` | Get recent N transactions (Stack) |
| `get_transactions_by_date` | Get transactions in date range (BST) |
| `set_budget` | Set/update category budget (HashMap) |
| `get_budgets` | Get all budgets with spending |
| `get_alerts` | Get budget alerts (>50% threshold) |
| `add_bill` | Add bill to queue (Queue enqueue) |
| `get_bills` | Get all bills in queue |
| `pay_bill` | Mark bill as paid |
| `delete_bill` | Remove bill from queue |
| `get_top_expenses` | Get top K expenses (Heap) |
| `get_top_categories` | Get top K categories (Category Heap) |
| `get_monthly_summary` | Get month summary (BST range) |
| `get_category_suggestions` | Autocomplete (Trie prefix) |
| `get_all_categories` | Get all categories |
| `undo` | Undo last action (Stack pop) |
| `get_dashboard` | Get dashboard summary |

---

## File Structure

```
/app
├── README.md                    # This documentation file
├── backend/
│   ├── server.py               # FastAPI REST API server
│   ├── requirements.txt        # Python dependencies
│   ├── .env                    # Environment variables
│   ├── cpp/
│   │   ├── Makefile            # Build configuration
│   │   ├── main.cpp            # Main executable entry point
│   │   ├── finance_engine.h    # Engine integration layer
│   │   ├── hashmap.h           # Hash Map implementation
│   │   ├── linkedlist.h        # Doubly Linked List implementation
│   │   ├── bst.h               # Binary Search Tree implementation
│   │   ├── heap.h              # Max Heap implementations
│   │   ├── queue.h             # Queue (FIFO) implementation
│   │   ├── stack.h             # Stack (LIFO) implementations
│   │   ├── trie.h              # Trie implementation
│   │   ├── finance_engine      # Compiled binary (Linux/Mac)
│   │   └── finance_engine.exe  # Compiled binary (Windows)
│   └── data/
│       ├── transactions.json   # Transaction data
│       ├── budgets.json        # Budget data
│       ├── bills.json          # Bill data
│       └── undo_stack.json     # Undo history
├── frontend/
│   ├── package.json            # Node.js dependencies
│   ├── yarn.lock               # Dependency lock file
│   ├── .env                    # Frontend environment variables
│   ├── tailwind.config.js      # Tailwind CSS configuration
│   ├── postcss.config.js       # PostCSS configuration
│   ├── craco.config.js         # CRACO configuration
│   ├── public/
│   │   └── index.html          # HTML entry point
│   └── src/
│       ├── index.js            # React entry point
│       ├── index.css           # Global styles
│       ├── App.js              # Main App component
│       ├── App.css             # App-specific styles
│       ├── components/
│       │   ├── Sidebar.jsx     # Navigation sidebar
│       │   └── ui/             # shadcn/ui components
│       ├── pages/
│       │   ├── Dashboard.jsx   # Dashboard page
│       │   ├── AddTransaction.jsx
│       │   ├── Transactions.jsx
│       │   ├── Budgets.jsx
│       │   ├── Bills.jsx
│       │   ├── Analytics.jsx
│       │   └── DSAInfo.jsx     # DSA reference page
│       ├── hooks/
│       │   └── use-toast.js    # Toast notification hook
│       └── lib/
│           └── utils.js        # Utility functions
└── test_result.md              # Testing results and protocol
```

---

## Demo Data

The application comes pre-loaded with demonstration data:

### Sample Transactions (15 entries)
- Income: Monthly salary ($5000), Freelance projects ($500), Dividends ($250)
- Expenses: Rent, Groceries, Transport, Entertainment, Utilities, Healthcare, etc.

### Sample Budgets (6 categories)
| Category | Budget Limit |
|----------|-------------|
| Groceries | $500 |
| Shopping | $300 |
| Entertainment | $150 |
| Food | $200 |
| Utilities | $150 |
| Transport | $200 |

### Sample Bills (4 entries)
- Internet Bill ($75) - Utilities
- Phone Bill ($50) - Utilities
- Gym Membership ($45) - Healthcare
- Netflix Subscription ($15.99) - Entertainment

---

## Time Complexity Analysis

| Operation | Time Complexity | Data Structure |
|-----------|----------------|----------------|
| Add Transaction | O(log n) | BST insert + O(1) HashMap update |
| Delete Transaction | O(n) | Linked List search + BST delete |
| Get All Transactions | O(n) | Linked List traversal or BST in-order |
| Get Transactions by Date Range | O(log n + k) | BST range query |
| Get Recent Transactions | O(k) | Stack top-k |
| Set Budget | O(1) | HashMap insert/update |
| Get Budget | O(1) | HashMap search |
| Get All Budgets | O(n) | HashMap iteration |
| Add Bill | O(1) | Queue enqueue |
| Pay/Delete Bill | O(n) | Queue search |
| Get Top K Expenses | O(k log n) | Max Heap extract k times |
| Get Category Suggestions | O(m + k) | Trie prefix search |
| Undo | O(1) | Stack pop |

---

## Viva Questions & Answers

### Q1: Why did you choose a Hash Map for budget management?
**A:** Hash Map provides O(1) average time for lookup, insert, and update operations. This is ideal for budget management where we need frequent category lookups. Unlike arrays or linked lists (O(n) search), HashMap allows instant access to any category's budget.

### Q2: Why use a Doubly Linked List instead of an array for transactions?
**A:** Doubly Linked List allows O(1) insertion at both ends and efficient bidirectional traversal. We can easily add new transactions at the front (most recent) and traverse backwards for history. Unlike arrays, we don't need to shift elements during deletion.

### Q3: How does BST help with date range queries?
**A:** BST maintains transactions sorted by date, enabling O(log n + k) range queries where k is the result count. For date ranges, we traverse from start date to end date using in-order traversal, skipping irrelevant subtrees - much faster than O(n) linear search.

### Q4: Why Max Heap for top expenses instead of sorting?
**A:** Max Heap allows us to extract top K expenses in O(k log n) time. Full sorting would take O(n log n). For finding top 5-10 expenses from thousands of transactions, heap is significantly faster. BuildHeap is also O(n) which is optimal.

### Q5: Why Queue for bills instead of Stack?
**A:** Bills follow FIFO order - bills added first should be paid first (based on due dates). Queue's FIFO behavior naturally represents this. Stack (LIFO) would pay newest bills first, which isn't practical for bill management.

### Q6: How does Stack enable undo functionality?
**A:** Stack's LIFO property is perfect for undo - the last action is undone first. We push each action to stack, and pop to undo. This is the standard approach used in editors, browsers, and most software with undo functionality.

### Q7: What advantage does Trie provide for autocomplete?
**A:** Trie enables O(m) prefix search where m is prefix length, regardless of total words. For autocomplete, this means instant suggestions as users type. Alternative approaches like filtering an array would be O(n*m) for n categories.

### Q8: What would you change if you had millions of transactions?
**A:** For scalability: 1) Use self-balancing BST (AVL/Red-Black) for guaranteed O(log n) operations, 2) Implement database indexing, 3) Add pagination for large results, 4) Consider B-trees for disk-based storage, 5) Use persistent heap structures.

### Q9: How would you handle concurrent access?
**A:** Add thread-safe mechanisms: 1) Mutex locks for write operations, 2) Read-write locks for better read performance, 3) Atomic operations where possible, 4) Transaction isolation for database operations.

### Q10: What's the space complexity of your implementation?
**A:** O(n) for all structures where n is the data count. HashMap: O(n) for entries, DLL: O(n) for nodes, BST: O(n) for nodes, Heap: O(n) for array, Queue: O(n) for nodes, Stack: O(k) where k is undo limit, Trie: O(m*k) for m words of avg length k.

---

## Environment Variables

### Backend (`/backend/.env`)
```env
MONGO_URL=mongodb://localhost:27017
```
Note: This application uses JSON files for storage, not MongoDB.

### Frontend (`/frontend/.env`)
```env
REACT_APP_BACKEND_URL=http://localhost:8001
```

---

## Testing

Backend API testing is performed using curl commands or automated test scripts:

```bash
# Health check
curl http://localhost:8001/api/health

# Get dashboard
curl http://localhost:8001/api/dashboard

# Add transaction
curl -X POST http://localhost:8001/api/transactions \
  -H "Content-Type: application/json" \
  -d '{"type":"expense","amount":50,"category":"Food","description":"Test"}'

# Get top expenses
curl http://localhost:8001/api/top-expenses?count=5
```

---

## License

This project is developed for academic purposes as part of Data Structures & Applications Lab.

---

## Contributors

1. Tapan Gupta
2. Stavan Rahul Khobare
3. Suraj Gupta

---

## Acknowledgments

- shadcn/ui for the beautiful React components
- Tailwind CSS for the styling framework
- FastAPI for the high-performance Python web framework
