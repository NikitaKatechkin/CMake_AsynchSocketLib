#include <IPEndpoint.h>

#include <iostream>

static uint8_t IPv4_ADDRESS_SIZE = 4;

namespace CustomSocket
{
	/**
	IPEndpoint::IPEndpoint(const std::string& ip, unsigned short port):
		m_port(port), m_ipBytes(new uint8_t[sizeof(uint32_t) / sizeof(uint8_t)])
	{
		in_addr address;
		int result = inet_pton(AF_INET, ip.c_str(), &address);
		//IF VALID IPv4 address was provided
		if (result == 1)
		{
			if (address.S_un.S_addr != INADDR_NONE)
			{
				m_ipString = ip;
				m_hostname = ip;
				memcpy_s(m_ipBytes, sizeof(uint32_t), &address.S_un.S_addr, sizeof(uint32_t));
				m_ipVersion = IPVersion::IPv4;
				return;
			}
		}
		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip.c_str(), nullptr, &hints, &hostinfo);
		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
			//host_addr->sin_addr.S_un.S_addr
			m_ipString.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &m_ipString[0], 16);
			m_hostname = ip;
			memcpy_s(m_ipBytes, sizeof(ULONG), &host_addr->sin_addr.S_un.S_addr, sizeof(ULONG));
			m_ipVersion = IPVersion::IPv4;
			freeaddrinfo(hostinfo);
			return;
		}
	}
	**/

	IPEndpoint::IPEndpoint(const std::string& ip, uint16_t port) :
		m_port(port), m_ipBytes(new uint8_t[IPv4_ADDRESS_SIZE])
	{
		in_addr ipAddrBuf;
		Result result = inet_pton(AF_INET, ip.c_str(), &ipAddrBuf) == 1 ? Result::Success :
			Result::Fail;

		if (result == Result::Success)
		{
			if (ipAddrBuf.S_un.S_addr != INADDR_NONE)
			{
				//IP ADDRESS WAS PROVIDED AS 0-255.0-255.0-255.0-255
				m_ipString = ip;
				m_hostname = ip;

				memcpy_s(m_ipBytes, IPv4_ADDRESS_SIZE,
					&ipAddrBuf.S_un.S_addr, IPv4_ADDRESS_SIZE);

				m_ipVersion = IPVersion::IPv4;
			}
			else
			{
				result = Result::Fail;
			}
		}

		if (result == Result::Fail)
		{
			addrinfo addrSettings = {};
			addrSettings.ai_family = AF_INET;

			addrinfo* hostinfo = nullptr;
			result = getaddrinfo(ip.c_str(), nullptr, &addrSettings, &hostinfo) != 0 ? Result::Fail :
				Result::Success;

			if (result == Result::Success)
			{
				//IP ADDRESS WAS PROVIDED AS WWW.GOOGLE.COM
				m_hostname = ip;
				m_ipVersion = IPVersion::IPv4;
				m_port = port;

				sockaddr_in* hostAddr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

				m_ipString.resize(16);
				inet_ntop(AF_INET, &hostAddr->sin_addr, &m_ipString[0], 16);

				memcpy_s(m_ipBytes, IPv4_ADDRESS_SIZE,
					&hostAddr->sin_addr.S_un.S_addr, IPv4_ADDRESS_SIZE);
			}
		}

		if (result == Result::Fail)
		{
			throw std::runtime_error("Invalid data was provided. Can not create an IPEndpoint object;");
		}
	}

	IPEndpoint::IPEndpoint(sockaddr* addr) :
		m_ipBytes(new uint8_t[IPv4_ADDRESS_SIZE])
	{
		if (addr->sa_family != AF_INET)
		{
			throw std::runtime_error("IPEndpoint::IPEndpoint(sockaddr* addr)");
		}

		m_ipVersion = IPVersion::IPv4;

		sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
		m_port = ntohs(addrv4->sin_port);

		memcpy_s(&m_ipBytes[0], IPv4_ADDRESS_SIZE, &addrv4->sin_addr, IPv4_ADDRESS_SIZE);

		m_ipString.resize(16);
		inet_ntop(AF_INET, &addrv4->sin_addr, &m_ipString[0], 16);

		m_hostname = m_ipString;
	}

	IPEndpoint::IPEndpoint(const IPEndpoint& copy)
	{
		m_hostname = copy.m_hostname;
		m_ipVersion = copy.m_ipVersion;
		m_ipString = copy.m_ipString;
		m_port = copy.m_port;

		if (copy.m_ipBytes != nullptr)
		{
			m_ipBytes = new uint8_t[IPv4_ADDRESS_SIZE];
			memcpy_s(m_ipBytes, IPv4_ADDRESS_SIZE * sizeof(uint8_t),
				copy.m_ipBytes, IPv4_ADDRESS_SIZE * sizeof(uint8_t));
		}
	}

	IPEndpoint::~IPEndpoint()
	{
		if (m_ipBytes != nullptr)
		{
			delete[] m_ipBytes;
			m_ipBytes = nullptr;
		}
	}

	IPVersion CustomSocket::IPEndpoint::GetIPVersion() const
	{
		return m_ipVersion;
	}

	std::string IPEndpoint::GetHostname() const 
	{
		return m_hostname;
	}

	std::string IPEndpoint::GetIPString() const
	{
		return m_ipString;
	}

	uint8_t* IPEndpoint::GetIPBytes() const
	{
		return m_ipBytes;
	}

	uint16_t IPEndpoint::GetPort() const
	{
		return m_port;
	}

	sockaddr_in IPEndpoint::GetSockaddrIPv4() const
	{
		if (m_ipVersion != IPVersion::IPv4)
		{
			throw std::runtime_error("IPEndpoint::GetSockaddrIPv4()");
		}

		sockaddr_in addr = {};

		addr.sin_family = AF_INET;
		memcpy_s(&addr.sin_addr, sizeof(ULONG), m_ipBytes, sizeof(ULONG));
		addr.sin_port = htons(m_port);

		return addr;
	}

	IPEndpoint& IPEndpoint::operator=(const IPEndpoint& copy)
	{
		m_hostname = copy.m_hostname;
		m_ipVersion = copy.m_ipVersion;
		m_ipString = copy.m_ipString;
		m_port = copy.m_port;

		if (copy.m_ipBytes != nullptr)
		{
			m_ipBytes = new uint8_t[IPv4_ADDRESS_SIZE];
			memcpy_s(m_ipBytes, IPv4_ADDRESS_SIZE * sizeof(uint8_t),
				copy.m_ipBytes, IPv4_ADDRESS_SIZE * sizeof(uint8_t));
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& outputStream, const IPEndpoint& obj)
	{
		outputStream << "--------------------------------------------------" << std::endl;
		outputStream << "~~~~~~~~~~~~~~~IPEndpoint class log~~~~~~~~~~~~~~~" << std::endl;

		switch (obj.m_ipVersion)
		{
		case IPVersion::IPv4:
			outputStream << "IP Version: IPv4" << std::endl;
			break;
		default:
			outputStream << "IP Version: Unknown" << std::endl;
			break;
		}
		outputStream << "Hostname: " << obj.m_hostname << std::endl;
		outputStream << "IP: " << obj.m_ipString << std::endl;
		outputStream << "Port: " << obj.m_port << std::endl;
		outputStream << "IP Bytes... " << std::endl;
		for (size_t index = 0; index < sizeof(uint32_t) / sizeof(uint8_t); index++)
		{
			outputStream << "\t" << static_cast<int>(obj.m_ipBytes[index]) << std::endl;
		}

		outputStream << "~~~~~~~~~~~~~~~IPEndpoint class log~~~~~~~~~~~~~~~" << std::endl;
		outputStream << "--------------------------------------------------" << std::endl;

		return outputStream;
	}

	bool operator==(const IPEndpoint& c1, const IPEndpoint& c2)
	{
		bool result = (c1.m_ipVersion == c2.m_ipVersion);

		if (result == true)
		{
			switch (c1.m_ipVersion)
			{
			case IPVersion::IPv4:
			{
				for (size_t index = 0; index < 4; index++)
				{
					if (c1.m_ipBytes[index] != c2.m_ipBytes[index])
					{
						result = false;
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		result = (result == true) ? (c1.m_port == c2.m_port) : false;

		return result;
	}

	bool operator<(const IPEndpoint& c1, const IPEndpoint& c2)
	{
		bool result = (c1.m_ipVersion < c2.m_ipVersion);
		bool isIPVersionEqual = (c1.m_ipVersion == c2.m_ipVersion);

		if (isIPVersionEqual == true)
		{
			bool isIPEqual = true;
			switch (c1.m_ipVersion)
			{
			case IPVersion::IPv4:
			{
				for (size_t index = 0; index < 4; index++)
				{
					if (c1.m_ipBytes[index] != c2.m_ipBytes[index])
					{
						result = (c1.m_ipBytes[index] < c2.m_ipBytes[index]);
						isIPEqual = false;
						break;
					}
				}
				break;
			}
			default:
				break;
			}

			if (isIPEqual == true)
			{
				result = (c1.m_port < c2.m_port);
			}
		}

		return result;
	}
}