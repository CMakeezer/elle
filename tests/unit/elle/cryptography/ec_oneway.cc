#include <iostream>

#include <elle/cryptography/oneway.hh>
#include <elle/cryptography/Plain.hh>
#include <elle/cryptography/Digest.hh>

#include <elle/format/hexadecimal.hh>

#include <stdexcept>

#define ASSERT(_condition_)                                             \
  if (!(_condition_))                                                   \
    throw std::runtime_error(#_condition_);

int main()
{
  elle::Buffer buffer{(elle::Byte const*)"suce.mon.cul", 12};
  elle::cryptography::Plain plain{elle::WeakBuffer{buffer}};

  elle::cryptography::Digest md5_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::md5)};
  elle::Buffer md5_result{
    elle::format::hexadecimal::decode("a81cf1c99a9833e027c9d5a33e9b85cd")};
  ASSERT(md5_digest.buffer() == md5_result);

  elle::cryptography::Digest sha_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha)};
  elle::Buffer sha_result{
    elle::format::hexadecimal::decode("51f973db2070983d562bd5d744eea4e5"
                                      "47de09fe")};
  ASSERT(sha_digest.buffer() == sha_result);

  elle::cryptography::Digest sha1_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha1)};
  elle::Buffer sha1_result{
    elle::format::hexadecimal::decode("1e408a2e1dace99ab9b3035f371417e6"
                                      "84c088fc")};
  ASSERT(sha1_digest.buffer() == sha1_result);

  elle::cryptography::Digest sha224_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha224)};
  elle::Buffer sha224_result{
    elle::format::hexadecimal::decode("909300b8bb726fbe15199967a2648855"
                                      "fb9bedf608c0446b366935b0")};
  ASSERT(sha224_digest.buffer() == sha224_result);

  elle::cryptography::Digest sha256_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha256)};
  elle::Buffer sha256_result{
    elle::format::hexadecimal::decode("b8bb6458a32b31734314985cfe300cec"
                                      "4d4a592a45c9859695bf2a2bdc9c0446")};
  ASSERT(sha256_digest.buffer() == sha256_result);

  elle::cryptography::Digest sha384_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha384)};
  elle::Buffer sha384_result{
    elle::format::hexadecimal::decode("a1ded70921fbe3afa87c856596de58a1"
                                      "a4e0cb43369c5df1ffe9a70744360f15"
                                      "74380c302077a5c92113a8f3d2b3cb68")};
  ASSERT(sha384_digest.buffer() == sha384_result);

  elle::cryptography::Digest sha512_digest{
    elle::cryptography::oneway::hash(
      plain,
      elle::cryptography::oneway::Algorithm::sha512)};
  elle::Buffer sha512_result{
    elle::format::hexadecimal::decode("3d22102fe74af8377f2fe9e48fa54481"
                                      "77b2621da77e9496edb667741c437c7a"
                                      "61f9f5436989d11a93830adc530235bd"
                                      "c34efb258fd05ca32688f5c809ea199f")};
  ASSERT(sha384_digest.buffer() == sha384_result);

  std::cout << "tests done.\n";

  return 0;
}
