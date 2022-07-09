# Check to see if git is installed
sudo apt install git

# Install nodejs
sudo curl -fsSL https://deb.nodesource.com/setup_18.x | bash -
sudo apt-get install -y nodejs

# Clone Lazap into .config
git clone https://github.com/lazap ~/.config/lazap

# run npm install
cd ~/.config/lazap
npm install

# Move the script to /usr/bin.
sudo mv ~/.config/lazap/scripts/lazap /usr/bin/

# cd to the directory
cd
