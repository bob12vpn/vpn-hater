# 🛡️ VPN-Hater

🌐 **VPN-Hater** is an innovative, open-source solution aimed at enhancing network security by blocking VPN connections. Distinguished from traditional inline methods, VPN-Hater is both reliable and cost-effective, ensuring uninterrupted network performance even in high-traffic scenarios.

---

## Environment

- Ubuntu 18, 20, 22

---

## 🚀 Key Features

- **💡 Out-of-Path Method**: Guarantees uninterrupted network performance.
- **🔒 Packet Injection**: Expertly blocks VPNs by injecting termination packets.
- **🔄 Supports Multiple Protocols**: Compatible with PPTP, L2TP, TCP Based VPN(OpenVPN TCP), and non-encrypted OpenVPN UDP.
- **🏎️ High Traffic Handling**: Maintains consistent speed under heavy load.
- **💸 Cost-Effective Solution**: Minimizes equipment replacement costs.

---

## 🛠️ Usage Guide

### 🌟 Setting Up

1. **Network Environment Setup**: Configure your network for port mirroring.

   ### 1) Using TAP device to mirror packets

      <img width="50%" src="https://github.com/bob12vpn/vpn-hater/assets/138478029/7a3eccf2-b988-4be5-b644-b9f3c2de428f"/>

   ### 2) Using Internet Router with port mirroring function

      <img width="50%" src="https://github.com/bob12vpn/vpn-hater/assets/138478029/2fd93ebd-773e-4f70-aef5-6ad4e6b1e2f9"/>
      
   ### 3) Using Switch with port mirroring function

      <img width="50%" src="https://github.com/bob12vpn/vpn-hater/assets/138478029/8d5f5a99-ba38-4f06-9331-d095b6619db4"/>


2. **Installation**:
      ```c++
      git clone https://github.com/bob12vpn/vpn-hater.git
      sudo apt install build-essential
      sudo apt install libpcap-dev
      make
      ```

      
   



### 📡 How It Works

- **PPTP & L2TP**: Strategy involves injecting termination request packets.
- **OpenVPN UDP (Non-Encrypted)**: Employs explicit-exit-notify packet injection.
- **OpenVPN TCP**: Utilizes parsed signature & injects FIN/RST packets.
- **TCP-Based VPNs**: Enhanced blocking via signature parsing.

---

## 📬 Support

🤝 For assistance, contact us at: [kusti0base@gmail.com](mailto:kusti0base@gmail.com)

---

## 📜 License

| Name | License | Copyright |
|------|---------|-----------|
| _Your Library Names Here_ | _Licenses_ | _Copyright Info_ |

---

## 📅 Change Log

🔍 Dive into our [Change Log](#change-log-link) for detailed updates.

---

## 📸 Example Usage

_Include captivating images or screenshots showcasing the system in action._

---

## 🔗 Additional Resources

- 📚 Explore [VPN Information and Documentation](#additional-resources-link) for in-depth knowledge.

---

**⚠️ Notes**:
- Replace `#installation-guide-link`, `#change-log-link`, and `#additional-resources-link` with actual URLs.
- Complete the License table with appropriate details.
