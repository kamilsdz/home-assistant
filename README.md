# Home Assistant for Raspberry Pi

# Instalation:
First of all - install Ansible
* `brew install http://git.io/sshpass.rb`
* `brew install ansible`

## Deploy Home Assistant server to Pi:
* Connect your Raspberry Pi to the same network
* `cd server/`
* `vim hosts` - provide raspberry's IP, `:w`
* `vim group_vars/all` - change the settings as needed
* run deploy: `ansible-playbook -i hosts site.yml`

App should be available at: <raspberry_id>:8123 

