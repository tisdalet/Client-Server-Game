# Battleship Game - Client-Server

This is a Battleship-style game managed using Git and GitHub. Follow the steps below to clone the repository, make changes, and push updates.

## Project Goals

### Part 1
- **Generate, Send, and Update Board**: Ensure the game board is generated, sent to clients, and updated correctly.
- **Handle Player Moves**: Capture player moves and update the board to reflect hits and misses.
- **End Game Condition**: End the game when all of an enemy’s ships are destroyed.

### Part 2
- **Implement Combat Mechanics**: Set up the core combat system for engaging with enemy ships.
- **Create Unique Ship Types**: Develop differently shaped ships without overlap, each with unique behaviors. Consider representing ships as objects and explore ways to store them with shapes intact.
- **Support Multiple Enemy Ships**: Allow the server to manage multiple enemy ships, choosing a random ship to attack and enabling multiple ships per game.

### Part 3
- **Scale Map with Difficulty**: Adjust map size and complexity based on selected difficulty level.
- **Restrict Enemy Types by Difficulty**: Limit certain enemy ship types to higher difficulties.
- **Introduce Enemy Waves**: Add waves of enemy ships to increase difficulty over time.
- **Implement PvP Mode**: Add an option for player-vs-player mode alongside player-vs-environment.

---

## Getting Started

### 1. Clone the Repository

To get a local copy of this project on your machine, follow these steps:

1. Open a terminal (Command Prompt or PowerShell on Windows).
2. Navigate to the directory where you want to clone the repository. Use the `cd` command:

   ```bash
   cd path\to\your\directory
   ```

3. Run the following command to clone the repository:

   ```bash
   git clone https://github.com/tisdalet/Client-Server-Game.git
   ```

4. After cloning, navigate into the project directory:

   ```bash
   cd Client-Server-Game
   ```

### 2. Create a New Branch

Before making any changes, create a new branch to keep the `main` branch clean:

```bash
git checkout -b your-branch-name
```

Replace `your-branch-name` with a meaningful name for your branch, such as `feature-new-functionality`.

### 3. Make Changes

- Make your changes to the project in your local environment.

### 4. Stage and Commit Changes

Once you have made changes, stage and commit them to the branch you created:

1. Stage all the changes:

   ```bash
   git add .
   ```

2. Commit the changes with a message:

   ```bash
   git commit -m "Description of the changes"
   ```

### 5. Push Your Changes to GitHub

Once your changes are committed, push the branch to GitHub:

```bash
git push origin your-branch-name
```

### 6. Create a Pull Request

1. Go to the repository on GitHub: [Client-Server Game](https://github.com/tisdalet/Client-Server-Game).
2. You should see a banner or button suggesting you create a pull request for the branch you just pushed.
3. Click **"Compare & pull request"** and provide a description of the changes made.
4. Submit the pull request for review.

### 7. Syncing with the Latest Updates (Optional)

If other team members have made changes and you want to get the latest version of the repository:

1. Pull the latest changes from the `main` branch:

   ```bash
   git pull origin main
   ```

2. If you are on a feature branch and want to sync with `main`, merge those changes into your branch:

   ```bash
   git merge main
   ```

3. Resolve any conflicts, if necessary, and continue working on your feature branch.

---

### Notes:
- Ensure you are always working on a new branch to avoid conflicts in the `main` branch.
- When pushing changes, always push your feature branch, not directly to `main` without proper review.
