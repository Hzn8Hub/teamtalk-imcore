/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:09:37 CST
 * @brief: 安全相关函数
*/

#ifndef TEAMTALK_IMCORE_SECURITY_SECURITY_H_
#define TEAMTALK_IMCORE_SECURITY_SECURITY_H_

#include <stdint.h>

namespace teamtalk::imcore::security {

/**
 *  对消息加密
 *
 *  @param pInData  待加密的消息内容指针
 *  @param nInLen   待加密消息内容长度
 *  @param pOutData 加密后的文本
 *  @param nOutLen  加密后的文本长度
 *
 *  @return 返回 0-成功; 其他-失败
 */


int EncryptMsg(const char* pInData, uint32_t nInLen, char** pOutData, uint32_t& nOutLen);

/**
 *  对消息解密
 *
 *  @param pInData  待解密的消息内容指针
 *  @param nInLen   待解密消息内容长度
 *  @param pOutData 解密后的文本
 *  @param nOutLen  解密后的文本长度
 *
 *  @return 返回 0-成功; 其他-失败
 */
int DecryptMsg(const char* pInData, uint32_t nInLen, char** pOutData, uint32_t& nOutLen);

/**
 *  对密码进行加密
 *
 *  @param pInData  待解密的消息内容指针
 *  @param nInLen   待解密消息内容长度
 *  @param pOutData 解密后的文本
 *  @param nOutLen  解密后的文本长度
 *  @param pKey     32位密钥
 *
 *  @return 返回 0-成功; 其他-失败
 */
int EncryptPass(const char* pInData, uint32_t nInLen, char** pOutData, uint32_t& nOutLen);

/**
 *  释放资源
 *
 *  @param pOutData 需要释放的资源
 */
void Free(char* pOutData);

}  // namespace teamtalk::imcore::security

#endif // TEAMTALK_IMCORE_SECURITY_SECURITY_H_
