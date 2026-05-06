import json
import os
from datetime import datetime

# --- Configuration ---
DATA_FILE = "todo_list.json"

# --- Data Handling ---


def load_tasks():
    """Loads the tasks from the JSON file."""
    if os.path.exists(DATA_FILE):
        try:
            with open(DATA_FILE, 'r') as f:
                return json.load(f)
        except json.JSONDecodeError:
            print(
                "⚠️  Warning: Could not decode the data file. Starting with an empty list.")
            return []
        except Exception as e:
            print(
                f"⚠️  Warning: An error occurred loading tasks: {e}. Starting fresh.")
            return []
    else:
        return []


def save_tasks(tasks):
    """Saves the current task list to the JSON file."""
    try:
        with open(DATA_FILE, 'w') as f:
            json.dump(tasks, f, indent=4)
        print(f"\n✅ To-do list successfully saved to {DATA_FILE}")
    except Exception as e:
        print(f"\n❌ Error saving tasks: {e}")

  # --- Core Functionality ---


def view_tasks(tasks):
    """Displays all tasks with their status and index."""
    if not tasks:
        print("\n✨ Your to-do list is empty! Time to add some tasks.")
        return

    print("\n========================================")
    print("          📝 YOUR TO-DO LIST 📝         ")
    print("========================================")
    for index, task in enumerate(tasks):
        status_icon = "✅" if task.get('completed', False) else "⏳"
        date_str = task.get('date', 'No date')
        print(
            f"{index + 1}. {status_icon} {task['description']} (Added: {date_str})")
    print("========================================\n")


def add_task(tasks):
    """Prompts the user for a new task description and adds it."""
    description = input("Enter the new task description: ").strip()
    if description:
        new_task = {
            "description": description,
            "completed": False,
            "date": datetime.now().strftime("%Y-%m-%d %H:%M")
        }
        tasks.append(new_task)
        print(f"\n➕ Task '{description}' added successfully!")
    else:
        print("\n⚠️  Task description cannot be empty.")


def mark_task_done_or_delete(tasks):
    """Allows the user to mark a task as done or delete it by index."""
    if not tasks:
        print("\n✨ No tasks to manage!")
        return

    view_tasks(tasks)
    print("\n--- Action Menu ---")
    print("Enter the task number to:")
    print("  - [D] Mark as Done (Completes the task)")
    print("  - [X] Delete Task (Permanently removes the task)")
    action = input("Choose action (D/X): ").upper()

    try:
        task_index = int(input("Enter the task number: ")) - 1

        if task_index < 0 or task_index >= len(tasks):
            print("❌ Invalid task number.")
            return

        if action == 'D':
            tasks[task_index]['completed'] = True
            print(f"\n✅ Task {task_index + 1} marked as DONE!")
        elif action == 'X':
            deleted_task = tasks.pop(task_index)
            print(f"\n🗑️  Task '{deleted_task['description']}' permanently deleted.")
        else:
            print("❌ Invalid action selected.")
    except ValueError:
        print("\n⚠️  Invalid input. Please enter a valid number.")


def main():
    """Main loop for the To-Do List Application."""
    tasks = load_tasks()
    print("==============================================")
    print("          📝 Command Line To-Do List       ")
    print("==============================================")

    while True:
        print("\n--- Menu ---")
        print("1. View Tasks")
        print("2. Add Task")
        print("3. Mark Task as Done (or Delete)")
        print("4. Exit & Save")

        choice = input("Enter your choice (1-4): ")

        if choice == '1':
            view_tasks(tasks)
        elif choice == '2':
            add_task(tasks)
        elif choice == '3':
            mark_task_done_or_delete(tasks)
        elif choice == '4':
            print("\n👋 Saving and exiting. Goodbye!")
            break
        else:
            print("\n⚠️  Invalid choice. Please enter a number between 1 and 4.")

    save_tasks(tasks)


if __name__ == "__main__":
    main()
