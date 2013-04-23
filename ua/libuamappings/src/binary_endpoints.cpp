/// @author Alexander Rykovanov 2012
/// @email rykovanov.as@gmail.com
/// @brief Opc Ua binary secure channel.
/// @license GNU LGPL
///
/// Distributed under the GNU LGPL License
/// (See accompanying file LICENSE or copy at 
/// http://www.gnu.org/licenses/lgpl.html)
///

#include "binary_serialization.h"

#include <opc/ua/protocol/endpoints.h>
#include <opc/ua/protocol/binary/stream.h>
#include <opc/ua/protocol/types.h>

#include <algorithm>
#include <memory>
#include <string>

namespace OpcUa
{

  GetEndpointsRequest::GetEndpointsRequest()
    : TypeID(GET_ENDPOINTS_REQUEST)
  {
  }

  GetEndpointsResponse::GetEndpointsResponse()
    : TypeID(GET_ENDPOINTS_RESPONSE)
  {
  }

  namespace Binary
  {

    //---------------------------------------------------
    // EndpointsFilter
    //---------------------------------------------------

    template<>
    std::size_t RawSize<EndpointsFilter>(const EndpointsFilter& filter)
    {
      return RawSize(filter.EndpointURL) + RawSize(filter.LocaleIDs) + RawSize(filter.ProfileUries);
    }

    template<>
    void OStream::Serialize<EndpointsFilter>(const EndpointsFilter& filter)
    {
      *this << filter.EndpointURL;

      if (filter.LocaleIDs.empty())
      {
        *this << uint32_t(0);
      }
      else
      {
        *this << filter.LocaleIDs;
      }

      if (filter.ProfileUries.empty())
      {
        *this << uint32_t(0);
      }
      else
      {
        *this << filter.ProfileUries;
      }
    }

    template<>
    void IStream::Deserialize<EndpointsFilter>(EndpointsFilter& filter)
    {
      *this >> filter.EndpointURL;
      *this >> filter.LocaleIDs;
      *this >> filter.ProfileUries;
    };

    //---------------------------------------------------
    // GetEndpointsRequest
    //---------------------------------------------------

    template<>
    std::size_t RawSize<GetEndpointsRequest>(const GetEndpointsRequest& request)
    {
      return RawSize(request.TypeID) + RawSize(request.Header) + RawSize(request.Filter);
      //return RawSize(request.TypeID) + RawSize(request.Header) + RawSize(request.EndpointURL) + RawSize(request.LocaleIDs) + RawSize(request.ProfileUries);
    }

    template<>
    void OStream::Serialize<GetEndpointsRequest>(const GetEndpointsRequest& request)
    {
      *this << request.TypeID;
      *this << request.Header;
      *this << request.Filter;
    }

    template<>
    void IStream::Deserialize<GetEndpointsRequest>(GetEndpointsRequest& request)
    {
      *this >> request.TypeID;
      *this >> request.Header;
      *this >> request.Filter;
    }

    //-----------------------------------------------------
    // ApplicationType
    //-----------------------------------------------------

    template<>
    std::size_t RawSize<ApplicationType>(const ApplicationType& type)
    {
      return 4;
    };

    template<>
    void OStream::Serialize<ApplicationType>(const ApplicationType& type)
    {
      *this << static_cast<uint32_t>(type);
    }

    template<>
    void IStream::Deserialize<ApplicationType>(ApplicationType& type)
    {
      uint32_t tmp = 0;
      *this >> tmp;
      type = static_cast<ApplicationType>(tmp);
    };

    //-----------------------------------------------------
    // UserIdentifyTokenType
    //-----------------------------------------------------

    template<>
    std::size_t RawSize<UserIdentifyTokenType>(const UserIdentifyTokenType& type)
    {
      return 4;
    };

    template<>
    void OStream::Serialize<UserIdentifyTokenType>(const UserIdentifyTokenType& type)
    {
      *this << static_cast<uint32_t>(type);
    }

    template<>
    void IStream::Deserialize<UserIdentifyTokenType>(UserIdentifyTokenType& type)
    {
      uint32_t tmp = 0;
      *this >> tmp;
      type = static_cast<UserIdentifyTokenType>(tmp);
    };


    //-----------------------------------------------------
    // UserTokenPolicy
    //-----------------------------------------------------

    template<>
    std::size_t RawSize<UserTokenPolicy>(const UserTokenPolicy& policy)
    {
      return RawSize(policy.PolicyID) +
             RawSize(policy.TokenType) +
             RawSize(policy.IssuedTokenType) +
             RawSize(policy.IssuerEndpointURL) +
             RawSize(policy.SecurityPolicyURI);
    };

    template<>
    std::size_t RawSize<std::vector<UserTokenPolicy>>(const std::vector<UserTokenPolicy>& vec)
    {
      const std::size_t headerSize = 4;
      std::size_t totalSize = headerSize;
      std::for_each(vec.begin(), vec.end(), [&] (const UserTokenPolicy& policy) {totalSize += RawSize(policy);});
      return totalSize;
    }

    template<>
    void OStream::Serialize<UserTokenPolicy>(const UserTokenPolicy& policy)
    {
      *this << policy.PolicyID;
      *this << policy.TokenType;
      *this << policy.IssuedTokenType;
      *this << policy.IssuerEndpointURL;
      *this << policy.SecurityPolicyURI;
    }

    template<>
    void IStream::Deserialize<UserTokenPolicy>(UserTokenPolicy& policy)
    {
      *this >> policy.PolicyID;
      *this >> policy.TokenType;
      *this >> policy.IssuedTokenType;
      *this >> policy.IssuerEndpointURL;
      *this >> policy.SecurityPolicyURI;
    };

    template<>
    void OStream::Serialize<std::vector<UserTokenPolicy>>(const std::vector<UserTokenPolicy>& vec)
    {
      SerializeContainer(*this, vec);
    }

    template<>
    void IStream::Deserialize<std::vector<UserTokenPolicy>>(std::vector<UserTokenPolicy>& vec)
    {
      DeserializeContainer(*this, vec);
    }



    //-----------------------------------------------------
    // ApplicationDescription
    //-----------------------------------------------------

    template<>
    std::size_t RawSize<ApplicationDescription>(const ApplicationDescription& desc)
    {
      return RawSize(desc.URI) +
             RawSize(desc.ProductURI) +
             RawSize(desc.Name) +
             RawSize(desc.Type) +
             RawSize(desc.GatewayServerURI) +
             RawSize(desc.DiscoveryProfileURI) +
             RawSize(desc.DiscoveryURLs);
    };

    template<>
    void OStream::Serialize<ApplicationDescription>(const ApplicationDescription& desc)
    {
      *this << desc.URI;
      *this << desc.ProductURI;
      *this << desc.Name;
      *this << desc.Type;
      *this << desc.GatewayServerURI;
      *this << desc.DiscoveryProfileURI;
      *this << desc.DiscoveryURLs;
    }

    template<>
    void IStream::Deserialize<ApplicationDescription>(ApplicationDescription& desc)
    {
      *this >> desc.URI;
      *this >> desc.ProductURI;
      *this >> desc.Name;
      *this >> desc.Type;
      *this >> desc.GatewayServerURI;
      *this >> desc.DiscoveryProfileURI;
      *this >> desc.DiscoveryURLs;
    }

    template<>
    void OStream::Serialize<std::vector<ApplicationDescription>>(const std::vector<ApplicationDescription>& vec)
    {
      SerializeContainer(*this, vec);
    }

    template<>
    void IStream::Deserialize<std::vector<ApplicationDescription>>(std::vector<ApplicationDescription>& vec)
    {
      DeserializeContainer(*this, vec);
    }

    //-----------------------------------------------------
    // EndpointDescription
    //-----------------------------------------------------


    template<>
    std::size_t RawSize<EndpointDescription>(const EndpointDescription& desc)
    {
      return RawSize(desc.EndpointURL) + 
             RawSize(desc.ServerDescription) + 
             RawSize(desc.ServerCertificate) +
             RawSize(desc.SecurityMode) +
             RawSize(desc.SecurityPolicyURI) +
             RawSize(desc.UserIdentifyTokens) +
             RawSize(desc.TransportProfileURI) +
             sizeof(desc.SecurityLevel);
    }

    template<>
    std::size_t RawSize<std::vector<EndpointDescription>>(const std::vector<EndpointDescription>& vec)
    {
      const std::size_t headerSize = 4;
      std::size_t totalSize = headerSize;
      std::for_each(vec.begin(), vec.end(), [&] (const EndpointDescription& desc) {totalSize += RawSize(desc);});
      return totalSize;
    }

    template<>
    void OStream::Serialize<EndpointDescription>(const EndpointDescription& desc)
    {
      *this << desc.EndpointURL;
      *this << desc.ServerDescription;
      *this << desc.ServerCertificate;
      *this << desc.SecurityMode;
      *this << desc.SecurityPolicyURI;
      *this << desc.UserIdentifyTokens;
      *this << desc.TransportProfileURI;
      *this << desc.SecurityLevel;
    }

    template<>
    void IStream::Deserialize<EndpointDescription>(EndpointDescription& desc)
    {
      *this >> desc.EndpointURL;
      *this >> desc.ServerDescription;
      *this >> desc.ServerCertificate;
      *this >> desc.SecurityMode;
      *this >> desc.SecurityPolicyURI;
      *this >> desc.UserIdentifyTokens;
      *this >> desc.TransportProfileURI;
      *this >> desc.SecurityLevel;
    }

    template<>
    void OStream::Serialize<std::vector<EndpointDescription>>(const std::vector<EndpointDescription>& vec)
    {
      SerializeContainer(*this, vec);
    }

    template<>
    void IStream::Deserialize<std::vector<EndpointDescription>>(std::vector<EndpointDescription>& vec)
    {
      DeserializeContainer(*this, vec);
    }


    //-----------------------------------------------------
    // GetEndpointsResponse
    //-----------------------------------------------------

    template<>
    std::size_t RawSize<GetEndpointsResponse>(const GetEndpointsResponse& resp)
    {
      return RawSize(resp.TypeID) + RawSize(resp.Header) + RawSize(resp.Endpoints);
    }

    template<>
    void OStream::Serialize<GetEndpointsResponse>(const GetEndpointsResponse& resp)
    {
      *this << resp.TypeID;
      *this << resp.Header;
      *this << resp.Endpoints;
    }

    template<>
    void IStream::Deserialize<GetEndpointsResponse>(GetEndpointsResponse& resp)
    {
      *this >> resp.TypeID;
      *this >> resp.Header;
      *this >> resp.Endpoints;
    }

  }
}

