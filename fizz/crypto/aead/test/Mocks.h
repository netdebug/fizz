/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/crypto/aead/Aead.h>
#include <folly/portability/GMock.h>

/* using override */
using namespace testing;

namespace fizz {
namespace test {

class MockAead : public Aead {
 public:
  MOCK_CONST_METHOD0(keyLength, size_t());
  MOCK_CONST_METHOD0(ivLength, size_t());
  MOCK_CONST_METHOD0(getCipherOverhead, size_t());
  MOCK_METHOD1(setEncryptedBufferHeadroom, void(size_t));

  MOCK_METHOD1(_setKey, void(TrafficKey& key));
  void setKey(TrafficKey key) override {
    return _setKey(key);
  }

  MOCK_CONST_METHOD3(
      _encrypt,
      std::unique_ptr<folly::IOBuf>(
          std::unique_ptr<folly::IOBuf>& plaintext,
          const folly::IOBuf* associatedData,
          uint64_t seqNum));
  std::unique_ptr<folly::IOBuf> encrypt(
      std::unique_ptr<folly::IOBuf>&& plaintext,
      const folly::IOBuf* associatedData,
      uint64_t seqNum) const override {
    return _encrypt(plaintext, associatedData, seqNum);
  }

  MOCK_CONST_METHOD3(
      _decrypt,
      std::unique_ptr<folly::IOBuf>(
          std::unique_ptr<folly::IOBuf>& ciphertext,
          const folly::IOBuf* associatedData,
          uint64_t seqNum));
  std::unique_ptr<folly::IOBuf> decrypt(
      std::unique_ptr<folly::IOBuf>&& ciphertext,
      const folly::IOBuf* associatedData,
      uint64_t seqNum) const override {
    return _decrypt(ciphertext, associatedData, seqNum);
  }

  MOCK_CONST_METHOD3(
      _tryDecrypt,
      folly::Optional<std::unique_ptr<folly::IOBuf>>(
          std::unique_ptr<folly::IOBuf>& ciphertext,
          const folly::IOBuf* associatedData,
          uint64_t seqNum));
  folly::Optional<std::unique_ptr<folly::IOBuf>> tryDecrypt(
      std::unique_ptr<folly::IOBuf>&& ciphertext,
      const folly::IOBuf* associatedData,
      uint64_t seqNum) const override {
    return _tryDecrypt(ciphertext, associatedData, seqNum);
  }

  void setDefaults() {
    ON_CALL(*this, _encrypt(_, _, _)).WillByDefault(InvokeWithoutArgs([]() {
      return folly::IOBuf::copyBuffer("ciphertext");
    }));
    ON_CALL(*this, _decrypt(_, _, _)).WillByDefault(InvokeWithoutArgs([]() {
      return folly::IOBuf::copyBuffer("plaintext");
    }));
    ON_CALL(*this, _tryDecrypt(_, _, _)).WillByDefault(InvokeWithoutArgs([]() {
      return folly::IOBuf::copyBuffer("plaintext");
    }));
  }
};
} // namespace test
} // namespace fizz
