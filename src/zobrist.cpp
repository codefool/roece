#include "roece.h"

// A list of random 64-bit numbers with a hamming distance of at least 1
// sides x piece types x squares
const std::uint64_t pieces_rn[2][6][64] = {
    // white king
    {
        0x1465b635ecee08aeull, 0x918e69669375f0e5ull, 0x5f8fe70722c7fc14ull, 0x9165978c241e26d1ull, 0xe9dc96ff0782739aull, 0x1dc9247baa6fa522ull, 0xcb6a46d314104e83ull, 0x185ca634dcaa8845ull, 
        0xc65ed083d8769226ull, 0x1448783a877c4e68ull, 0xa5b09027ed7381c1ull, 0x21a0f26e57e21831ull, 0x388ddcc6c625d57cull, 0x6d77491a8408360dull, 0x25672e710d9a17ecull, 0x06f3f8d09164c53cull, 
        0x4a7c3be204033874ull, 0xc9a73079197f39eaull, 0x84fa0b17a4a896a4ull, 0x80f35d45a9962bd9ull, 0x8075ccb8c813b1f3ull, 0x4f45110b32b7db5full, 0x50334c3e216e0597ull, 0xf74370c2af96de3full, 
        0x51dab6988c5502c0ull, 0xaa2d97027ef379f9ull, 0x4baf159c07b25b82ull, 0x8d580d57d26487f6ull, 0x3f300ca3131eb617ull, 0x5410db3b3282d737ull, 0x571db47ae654cab2ull, 0x773158a028ad6ea7ull, 
        0xae8250c0a55b9a30ull, 0x037e6e25147396f6ull, 0xbacf04089d1a24f0ull, 0xc8da2c1cdfa69c5aull, 0x2a1d0bb7fef788c4ull, 0xea37f92055abba00ull, 0x830175e916ea4f14ull, 0xb2e77a44af1a2802ull, 
        0x0fce56fd8dc2cfdcull, 0xdb039c25124efc34ull, 0x5470a3b17a612f43ull, 0x9c0b39878ad0a8b5ull, 0x44b81ab6f9f554d9ull, 0xc487e7036e4e9a7eull, 0x5c1a5dcd8bef210full, 0x88566574f92583b6ull, 
        0x5c8013c5d657f33aull, 0x27eacfd29a0547cbull, 0x70e78c9db4785e24ull, 0xd480a678141bd3a8ull, 0xf88f3c481602e8feull, 0x881ec38f088c2891ull, 0x08507d0a518158b5ull, 0x182aa606d319afbdull, 
        0xbf38641c06df81e1ull, 0x27cfe585040f42f8ull, 0x64cf2b73b1f13826ull, 0xe2b2aa8e1620ac7full, 0x0e51ab327d54a8f8ull, 0xc3fc7ba27c26d388ull, 0xe971fdc2cb0813aaull, 0x598544c2da034bddull, 
        // queen
        0x465679432865012full, 0xfeafeab131a16718ull, 0x8deaa21f82dd362aull, 0x32b59b9da97ec0dbull, 0x03d887f350617559ull, 0x0239d0700338d617ull, 0xafbd6a106ac23231ull, 0xff371bcf185ee607ull, 
        0x78806be0af9a8a82ull, 0x523bb79e2379d6fbull, 0x6a563f6593948852ull, 0x63a0f7ec89e60df9ull, 0xc2291fd3eb9698ecull, 0x9f0eb407d152ed94ull, 0x5ae6110fffefe509ull, 0x05ccb9ddcd8bd646ull, 
        0x5a2d013dca78bc90ull, 0x3efb5dcc7c549e71ull, 0xd2cabc8107f5fb6aull, 0xbfda819cd24ed86dull, 0x497f661bd41ae2dcull, 0x2cc4d50a03465f6eull, 0x8376e2d90a9ec906ull, 0x858470947db3f6abull, 
        0x4a393bfed0bf2196ull, 0x146f4b3a852c67deull, 0xf2fb98da4696303full, 0x1d63555e2b81fa0aull, 0x77ef0a37417a3ab0ull, 0x4dd7a8466bbb36b2ull, 0x7520f7583d05fe60ull, 0xc18f3508cc6a5e27ull, 
        0xacbb8a77cbd817fdull, 0x8293251d4e02df99ull, 0xbc46f8ef542a5857ull, 0x6863768a1e6d53e7ull, 0x0ae32bccbf5ffabcull, 0x643a4b9870fffc9dull, 0xef5874a214c70d1cull, 0x579d87f6aae8f85eull, 
        0x33c0ae716348a5fcull, 0x90035d39162d32f2ull, 0xbbca46d3d6f1934cull, 0x61ef1338b14f57b4ull, 0xea87158c0b96f1efull, 0x9e376dbd4211927full, 0xf01e581a88f98fc4ull, 0x4d30eba20152526bull, 
        0x44b4f4032fe30e31ull, 0x4f21c20fd10f94ceull, 0x752e60300a855b41ull, 0xa4a3ab0be8cb504aull, 0xe8c2d22777d5c0ecull, 0x0b1e987f876d39afull, 0x86282359761a0763ull, 0xc03673a2216ef65aull, 
        0x9f69177268997838ull, 0x1a6fc9bf1a0e36d6ull, 0x9b6d9c5170fab3d8ull, 0x77f92ed99db9de6bull, 0xd00788fa8daf537eull, 0x029a32165d5b24cfull, 0xfe84fe01368246d2ull, 0xb0c92e42acbe0287ull, 
        // bishop
        0x03f9258de19409deull, 0xa1262bec6d144e71ull, 0x7c44926e14550818ull, 0x40807e41d4899aa2ull, 0x179c6b28deb9abb9ull, 0x8e3645ee3e6ad3f0ull, 0x5ba85f83285508b5ull, 0xe3b819ef93d53567ull, 
        0x063ed288952a4847ull, 0xfd6f6868ddd2b3d2ull, 0x4dd4171790bfe12dull, 0x8db06cf57ade6516ull, 0x386bdcf25bbeedd8ull, 0x618f96c75979920dull, 0xe2666e735afe620bull, 0x7bc719efbc9cf880ull, 
        0xf50baf4c18b95fd1ull, 0x00fba9ad17a66e99ull, 0x578b3d323f99db54ull, 0x490d6ee28a36628aull, 0x8209eabad8ef4bbaull, 0xbd6a85e9c579106dull, 0x06ed851785576d0dull, 0xe43b9feb801ba85dull, 
        0xf16a60f1987514e8ull, 0x206d26904ddec281ull, 0xa874a0ebac5291c4ull, 0x6a81947c6dd256d6ull, 0x426df6e8e2d15b74ull, 0x7491ded2ea94bfb1ull, 0x65645d693fd17809ull, 0x66a4ef59bc7aca86ull, 
        0xeacbfb8ef1084cc7ull, 0x230e23fd41e27a11ull, 0x72bbe1d71adc1782ull, 0x58c05ff37a5cfc7aull, 0x4fe1c8e47203e4afull, 0x7a59d47bbb467b0cull, 0x6c2e0906e7d3b338ull, 0x8b33a8a368f6c10aull, 
        0xda0ef98aa5c22c2full, 0xf540d03926302631ull, 0x40bb12d4b0c84f05ull, 0xfa092005e28e87d3ull, 0x540da5a84e21cbdfull, 0xec7190740af26f1full, 0x950323c0e85fe3d0ull, 0x4512975259a952a0ull, 
        0x23f8fcd244c0fca2ull, 0xabfea15dfaf18a50ull, 0xb04cbcebcb8a4f96ull, 0x77570a4b8a761761ull, 0x9c15d82e782df2f3ull, 0xd2b55ef685524143ull, 0x1545370b59b6f6b8ull, 0x6d55201af3ab3daaull, 
        0x974f9531d709eaedull, 0x10d426a4d140ebaaull, 0x69f5975c02ffc0a0ull, 0xdd44a249f0bf6abbull, 0x4743c15345c51b38ull, 0xb1d38844e6ec2d28ull, 0x824e2b67a2b57970ull, 0xbe740e983ce92d0aull, 
        // knight
        0x8c2f8be4ae201dc2ull, 0x18f052e982d8556full, 0x704cbc375529a97aull, 0xef0ee7213cc0a63cull, 0xa5f182a80780eba8ull, 0x92ccaeebb9928309ull, 0x274b424146fbf248ull, 0xb0997bddac674291ull, 
        0xfc3890848b1b3f67ull, 0xbd06dbf79550cc8cull, 0xaf6e71101abdb1d2ull, 0xd70c50104c198596ull, 0xa117196789d4ed0bull, 0x04ad3d31a3484f0aull, 0xdf1fea7b227a7229ull, 0x463ce2ee0c5e9747ull, 
        0x2ec3cf979e74767dull, 0x1b5eb010bb40d566ull, 0x46b6e0287ee1c765ull, 0xa9c4f322e21e3144ull, 0x70c41eae32f314b7ull, 0xfe3b291aa74971a7ull, 0x0e7705bc0046ec7eull, 0x6c6e30599730a55eull, 
        0x0f128a4346af784cull, 0x2a8c0d330088d18bull, 0xfc308e1dbb3f383eull, 0x39214b69b71fc156ull, 0x54a26c55f5e75799ull, 0x2f6e0c7458054540ull, 0x9a18905ffe850865ull, 0x5c80c2648596e990ull, 
        0x5114c04bacfb1f72ull, 0x128340bea4257a7aull, 0xb46f9456429e7fc5ull, 0xf49a9cecabd6aa7eull, 0x592b7311f02b418eull, 0x64bc739b99625ae9ull, 0x01e08225ffb0c836ull, 0x7e570048d0f876beull, 
        0x880b6fb1016ca4eaull, 0x5a7c49a67db4cd08ull, 0xd870b0beaee12bf5ull, 0xaa1b77f754dc13f8ull, 0x66297f8586ff4bc9ull, 0x7f49428d4e32e5ffull, 0x3bdb0bd6dd14bb0cull, 0x6d6dd13bd16a9dafull, 
        0x59bee3526788d8acull, 0xafc53c63dabd79d7ull, 0x956823401044e0cbull, 0xac4647c4ec4cde13ull, 0xedeb1a1ac6f9446eull, 0x67d935381ca9ea5full, 0x10d3d1eb2c881584ull, 0x09f96fec957d3fc6ull, 
        0x1181864892fc269eull, 0x02834158ef35362full, 0xe96a871031c62ce1ull, 0xc9ddacaa40816ac4ull, 0x513cb33d8d4c0674ull, 0x940e65f8831733b4ull, 0xfea3987ab4a5fcffull, 0xb5f9e66625f6e5dfull, 
        // rook
        0xe088e1c6a8c13548ull, 0x4e1075c9853b2eb9ull, 0xe6b93166a8de7b34ull, 0x85eef70f72b4b1ebull, 0x68f57994e62d4887ull, 0x5726e00b63c4ac93ull, 0xc2498772d851cf14ull, 0xc49a9d016f1a5fc2ull, 
        0x807921869e1a9616ull, 0xd4264dfa7ec5d876ull, 0xc5300d0e56fb7b8aull, 0x8069c1bddddb694bull, 0x329feebfc038b0e8ull, 0x317194eab2f7377aull, 0xa190fa36eb99e0fdull, 0x45af798136459c1cull, 
        0xf9e5f8c98f38e3a5ull, 0xcd0a851a8f0f1fa2ull, 0x7151931001be2c76ull, 0x194e87c0dee53aeeull, 0x912567cfdb2c5f12ull, 0x7eab8bb7c5f216aaull, 0xa9b5fc6c89ac6012ull, 0x732b15c0642a7114ull, 
        0x792fc63713291fbeull, 0x1d529f6062e0ea6bull, 0x83db4e54ee8536b4ull, 0x95f88a0fcced4361ull, 0xcfa3b022ff70cf3full, 0x8db3a6ef6522f3e3ull, 0xcb3f27c0c2fdcb9full, 0xd3e2754c0a36e528ull, 
        0x957f7579928212e5ull, 0xf925e52c00903ef3ull, 0xabc129ac5e0786c4ull, 0xde5fe1c8db28cc15ull, 0x4fa5e49a226e5cafull, 0x3708c82233d401b9ull, 0xbe2cb6b92b56b232ull, 0xb97b44be66d4448bull, 
        0x0144230ff9da5c3dull, 0x7dda681288bf9d0bull, 0x9b2595befe59e6f4ull, 0xe8e0a51c4a7ec2efull, 0xaaf580c50366bb67ull, 0xc417356d5d67122cull, 0x8445bdfe0943428bull, 0xc73e6cfa7ab0b340ull, 
        0x2ba9a0139f1d7291ull, 0xe23a267be38867deull, 0x8e5b30e74cbec0fbull, 0xe686f25f935e3643ull, 0x82c25fd9aa91e901ull, 0xe7273e2d6b890ad5ull, 0x90bb4235730d9955ull, 0xd09cbc633431ad65ull, 
        0xeb3edc1167e06f20ull, 0x2e92e39b8edf96a6ull, 0x4fd932889ed15465ull, 0x509e102e9dcfca9aull, 0xa6c55dcb334106baull, 0x62c121b68d88b80cull, 0x8aa1420f134f209cull, 0x7007f498fc46f3f5ull, 
        // pawn
        0xa286a07d593f7ea0ull, 0x33496e7c9dcc255dull, 0xca51135607b54649ull, 0xa4dfda28df9d3af3ull, 0x8f8a68b069f23670ull, 0x3430236ec3d4655aull, 0x28395845f192af7aull, 0x4161e1a415a29e48ull, 
        0xa13d1c452abaf29eull, 0xff4713b749fdef73ull, 0x10a938fd4665f14dull, 0xbc7dd70cc9d10acfull, 0xf2de0e65ed369f72ull, 0xd2eedd9e6c700cf5ull, 0x0ccbed8032ee2a37ull, 0x54021db7e41cd24aull, 
        0xe5a4a9d01dea09ccull, 0xf817466835d0ec98ull, 0x1f790149e670fbefull, 0x6e8896b5079657d8ull, 0x4dff95b70af7bc95ull, 0xd6c7dc58321154e4ull, 0x4179571daca1354eull, 0xd75685b08606616eull, 
        0x3cd31987cc434240ull, 0x941be1c0a2df4219ull, 0xd73956216d15eec6ull, 0x810b608c97a1a8ccull, 0x8339f19ce581eeb1ull, 0xb75225f35e8a2bb8ull, 0x8b7e85c784b20602ull, 0x5361f955051cb6a7ull, 
        0x94d39e43aed8d414ull, 0x791408732e9e8f8full, 0xf913de406a36fc95ull, 0xa206c6e2e8c48d23ull, 0xe7fe72e1b54688deull, 0x4c250613c8f7d6c6ull, 0x228c4e326125a482ull, 0x983ac25d3c92123cull, 
        0x997b0b58e769ba1full, 0xd92e42c68e28194aull, 0xe5dae23f105b15bcull, 0xc01216bfe32c265full, 0x37be1254937c4f8aull, 0xfbe5e6360113eeb7ull, 0xb6c939865cb19c5dull, 0x9208bbbcef0e8ef4ull, 
        0xf2b6ad8438059ed6ull, 0x303288f9a9b2fea4ull, 0x17799819b4c5d6bfull, 0xa776c8ddf9a47fd5ull, 0xb9d39e7b219dbea5ull, 0xe91dd484cf955119ull, 0xbdb5d5e3e35c79a2ull, 0x961261fc4734f508ull, 
        0xcf9ae1bb156e18a6ull, 0x90fe44d7b68ae3ccull, 0x78ca747908de39aeull, 0x4089927bd06e7f36ull, 0x092d3b758c0e8a0bull, 0x01b789562a07df2eull, 0x17f094e23c0a1b37ull, 0xebcbe68317df7d21ull, 
    },
    // black king
    {
        0x0376ff5ddfb7d164ull, 0xe2041e87029262cfull, 0x7e0155e68e2462bbull, 0xe493c527022284a4ull, 0x96057046a3598d3bull, 0x8bf69259869afea0ull, 0xa4042e8910b1458eull, 0xf095833bd9f4392eull, 
        0xb2be833d8d347cc8ull, 0x68d12e7f337b39ecull, 0x6cdeb596627ffdf3ull, 0xf85cf239844a78a7ull, 0x8504d8bee01b6577ull, 0xd833535fa01ecfc2ull, 0xda241a20097db3d5ull, 0x7790e3bc276b81faull, 
        0x707746d31658f03bull, 0x259d6b26543d0048ull, 0x45a0e92baca689fcull, 0x8335131ac7aab3b7ull, 0x5737404a4bed7809ull, 0x0c9daba78daa63d7ull, 0xecbfaaed4dd5c339ull, 0x57c583f4d71d1020ull, 
        0x3be420f572260501ull, 0x741274df5a248a9cull, 0x2bb8033762f8b0b2ull, 0x028181e868cb18bcull, 0xcbf08e54c21bdd44ull, 0xe4a74e7581db0728ull, 0x48e756fa8b4d6a48ull, 0xa7098a5ef4ad952aull, 
        0x94f3145598b2a48bull, 0x1ea2126712972727ull, 0x308832d21961e769ull, 0x511d85c3edafbee7ull, 0x23bf99506e522ff2ull, 0x3988a99c65a986e2ull, 0x904b2b74ba8ca91eull, 0x792525da6cbb35c9ull, 
        0x9866aedc52f8287aull, 0x4a8b2fdb50d00960ull, 0xda5dd1544a91cf67ull, 0xfc2b33ed23237031ull, 0xdce30894bb366325ull, 0x0f26ca6e2295549full, 0x96b8ef86ffc12b39ull, 0x5be6446c0d4be1d7ull, 
        0x3526372c142133cdull, 0xf270b85fcdcecceeull, 0xb4b699f8d11ff172ull, 0x20bfbca3ddf95ba6ull, 0x1cc7cfdd91d9d628ull, 0x3ceba45f266215ffull, 0x2a59c20733682b4aull, 0x973303d53624ee8dull, 
        0xd8f4575fa228bd93ull, 0xec7afd9b8e6d7c91ull, 0xf7a1f0991faeb670ull, 0xcf18f50e1ef5d8f3ull, 0xc46eab62db03d226ull, 0x3545ea1c65a733e6ull, 0x8b227305b40cd56cull, 0x81fd80cef6d173c9ull, 
        // queen
        0x1e5af1031267cc29ull, 0x8fd3a97e355e005bull, 0xe74151726f96f7e4ull, 0x440dc434d555b411ull, 0xeae36f25eac2a782ull, 0x8543827bdc50bdfcull, 0xc249646f1620a898ull, 0x0bfb50001bb90170ull, 
        0x3a0ae6e573587552ull, 0x13da9990525a5b31ull, 0x5cecffc8ba67a759ull, 0x061f00c663b70874ull, 0x8ca6e448784f2bf1ull, 0x2fc014195ff46da0ull, 0x81cdf303dc3b2853ull, 0x3f378be2d33e5b88ull, 
        0x9fd647bd76a737aaull, 0x2a683c7a2d9e400bull, 0xc13d7703a46c4740ull, 0x76ab7b05321121e6ull, 0xd148ee5f397ddc75ull, 0x0c1313c1f719eae2ull, 0x0e7c84e5c3834415ull, 0x50f501fdfe25fbf6ull, 
        0xfab29b6508c192a8ull, 0x4d9239467c8055efull, 0xdec36fe8eb0c9b07ull, 0x58502a7e954805a1ull, 0x134c90ea2edc4e3eull, 0xb8009eae3ce7058dull, 0x9b4ae228e560d9ccull, 0x281ce55aded96461ull, 
        0x6867858d1f1c99bcull, 0x77550f960c29c6d5ull, 0x4fbb418b03816e2eull, 0x468685d390116c36ull, 0xb10c39e4a5109defull, 0xf22a16b77c75849dull, 0xa3d03d5beefedbdeull, 0x2707466161b94b17ull, 
        0x2685807d012aa285ull, 0xaa8880c8db4df04full, 0xf49ac8372b19f087ull, 0x104d27820496f3c6ull, 0xb19ef5dc000a4114ull, 0xbe7dc5f4ab2068c4ull, 0x9485513b0af034ccull, 0x7f13ce39ea1afa7bull, 
        0xe1e1fbdfe377c8d6ull, 0x501b0608cbf89a49ull, 0x8c382b03624dd696ull, 0xf3a3697c5b67f0ecull, 0xc66041633516d92dull, 0x29b65fe733b3dd3bull, 0xe6421b6f2783915dull, 0xbf14653a31b4b826ull, 
        0x086cdc7147c7a6b2ull, 0x81c0d2b6191b034cull, 0xe20d4cc7abc22559ull, 0xddab454e6dcdaac2ull, 0x610d069d1a7d5725ull, 0x7c60ba71b11b3b8dull, 0x6b8a7fea4f5b09b7ull, 0xc8eb6d71696db85eull, 
        // bishop
        0xefd2d5ec4211ddc8ull, 0xe99243af08f50ccbull, 0xef4d9cbeecbbd4acull, 0xdca84960c5a30444ull, 0x76f825e00f09bb95ull, 0x5807cc23b98fa821ull, 0xa906c54e0a713baeull, 0xb1023cca7b903cdfull, 
        0xf3034edfd68b2836ull, 0x7b563f8929a6dad0ull, 0x373978b227a403e2ull, 0x82d38c801b45a875ull, 0x1243902245eb4defull, 0x416f4dbe506437aaull, 0xbafd1273a3efcea8ull, 0xe431bb8e89d8298dull, 
        0x3f9baf64e69d8277ull, 0xfee75732e0a92b64ull, 0xa8514bc873908d5eull, 0xd38a8980b115272full, 0x423b256e579a0867ull, 0x355301f2d34e08b3ull, 0x25ce3786d472c522ull, 0xb500594a2c28df3cull, 
        0x1cf5026f42b3f60eull, 0x0f7aef508a285463ull, 0x99620f82449f6378ull, 0x07ee92b11df1b712ull, 0x5655f000e30ce9e7ull, 0xa8abe41dc3aabf07ull, 0xb5a10555ccdb2d8cull, 0x9fccf64ce5181ea0ull, 
        0x9a9c69f183eb29f8ull, 0x7c6a9c3bc8cf561full, 0x1f3f1814584ea9a8ull, 0x4286cdf714ff6f8bull, 0x7afa8ed506aba7a2ull, 0x6d6fe453fc9192efull, 0xe8993eb7fc6b3c4aull, 0xf48e388bfa0f30b9ull, 
        0x8c41fbf568bfc1a5ull, 0x2fca71480379eeeaull, 0x29b3cbe15bed4bc3ull, 0xefce05e62b676f6cull, 0x44bcf4bd6d343cadull, 0xced18c7bed93835bull, 0x3668655a2855c4c6ull, 0x6955a2f40f101cbdull, 
        0x0f1b523b4057b1d4ull, 0x50e6226c6e927f32ull, 0xc1f8928355f248dcull, 0xa7930574f1eb01e3ull, 0x37d356afee8c80b4ull, 0x85df8138734b74a2ull, 0x1aa24a30aff5190dull, 0xf0c4dca1380c2023ull, 
        0x3ff8cce8892ccfefull, 0x2ba795bfd17f7638ull, 0x633cdc29abec54beull, 0x7c4a23311128645full, 0x758bf079555e025eull, 0xacedaecbe7e91400ull, 0x4774837a6bb47320ull, 0xbfb94e47cf718071ull, 
        // knight
        0xf4e3be528492e577ull, 0x9a36ec42c29e046dull, 0xe0dcc8215c4e807eull, 0x0b75226bcae75367ull, 0x02f084e2ceebc62cull, 0x51e82cd261f564f0ull, 0xf48b1f6eb8e320cbull, 0x7e59d977627dfa34ull, 
        0xabd5c3e25599eb87ull, 0x37f752091baa5f5eull, 0x4f96c0c2f52b499cull, 0x7126b04768223d06ull, 0xa6d9d02ee61230b5ull, 0x411cbe2db94e8d81ull, 0x16a39e28a468f5bcull, 0xbf2b9a8e15e47f21ull, 
        0x77e529a157df7d1bull, 0x2c97048d3da647b8ull, 0xb33a747489999279ull, 0x99b553b81314501dull, 0x0792f223160ac65aull, 0x86d79b5560f7c62aull, 0xa565e2e118099dddull, 0x0108daf93285d5b4ull, 
        0xf6ab5fa6b18f85d6ull, 0xf59ae7071a124890ull, 0x3c331463d7297f32ull, 0x1c2fe854e0cc9538ull, 0xdc4ee348034f1409ull, 0xf82a259ca7926a07ull, 0x3fdad7faa050a937ull, 0x164680511b259de4ull, 
        0x4fab3fabe714ae51ull, 0x1a55ce4c8679f565ull, 0x23175112283eefacull, 0xd1359d35c0cb5895ull, 0x7b44af62f52c8b6eull, 0x1cb4ab30118fc08eull, 0x8d1c385aeb6ed6c0ull, 0xa9ac831b29f33261ull, 
        0xeaf46883a57f387full, 0x61852109fae96c00ull, 0xa1da0cae987535a2ull, 0x32a6be1b2929ab40ull, 0x5a6a2801aefdb2f8ull, 0x71ec02d6ed17093full, 0x83639c0ba7fa9245ull, 0x7435d18f115bd817ull, 
        0xdc35666bc9058821ull, 0xd2b9278847058dbaull, 0xd24b564aea46343eull, 0x4ff02e9530533ce4ull, 0xca9a7270da7f4908ull, 0x10818879aa4754b4ull, 0x1007f2b149cbb278ull, 0xe049740ddb7a8218ull, 
        0x69937739bd130c4full, 0xa09d71e573655920ull, 0xa3d034b494551fbcull, 0xf564f5c99b773eb2ull, 0x484a1942fedef1f9ull, 0x66c44ba02f96b5faull, 0xa58114d2f38a5977ull, 0x9dedd58a00649c8cull, 
        // rook
        0x4ba041641e6c775dull, 0x45fae2d238916c5dull, 0xb8227ec87a360343ull, 0x9f7647a00436624full, 0xc1ad99591d50ed5bull, 0x979923b509d42fd7ull, 0x46e5b521a0bcf92dull, 0x0d634929456efcc7ull, 
        0x688822c132d07285ull, 0x04fcae8d2289b369ull, 0xd778d36e4ba3f320ull, 0xc7a68910d8d32744ull, 0x977e2acbd28c63bfull, 0x7d73fd131089915aull, 0x0a8e09da3571c08bull, 0x3a44d3151f6ab487ull, 
        0x6f36c019789124f2ull, 0x8a8cb83cf2edf8c0ull, 0xe1976520b66f09d8ull, 0xd7b039ac91694d71ull, 0x0c6de175ef638704ull, 0x7ccdb7f18777c029ull, 0x61ccac58e094c533ull, 0x982b3cdede667107ull, 
        0x434a87e03570528eull, 0x4bdd6aa99004153eull, 0x61b41cf2adb3d57aull, 0xdebda662037cc00full, 0x89caeaed8733769dull, 0xcd5083064fb5c798ull, 0x91fd9a82751bd267ull, 0x622f50a68a6027cfull, 
        0x89cb7c58065ae8b5ull, 0xaa7bb8486e3e235full, 0xb906982f894e7571ull, 0xbb1e9c09f476c801ull, 0x1601288ac2953576ull, 0x422bae26a943ecc8ull, 0x9708c79172713d85ull, 0x9be4db04ca4809b0ull, 
        0x518a1887c15e5cddull, 0x5cb9072f489248fbull, 0xef27a76e937d519bull, 0x238e297f58dd9365ull, 0xc0ff27e5915a4416ull, 0xdae5873f1cc8b0cfull, 0x0cf097a1389767bbull, 0xd413117d92a3d982ull, 
        0x99015dd5789da0f7ull, 0xce6445dbd7ce1116ull, 0x87339c3e92ba045cull, 0xcf85e91814837aa2ull, 0xfdd1651eb5ccf1faull, 0x8e2e9812fd367d24ull, 0x97d53978bbdcb5c4ull, 0xb16dc19a2a1c598full, 
        0xdf081c595ec1ab15ull, 0x9911a458458252edull, 0xd49e236af2174fd2ull, 0x9ecce5a4959dd0bdull, 0x102e5a33dc53afc8ull, 0x60fbf4ee6e6e84b5ull, 0x33eb399af3a65268ull, 0x22de51c28687b8b7ull, 
        // pawn
        0xde0bd507c9cac6a4ull, 0x3dc0cda01e4f1246ull, 0xf9b799ee0c22b7dfull, 0xe1f649984640ed08ull, 0x0327ba4701e11410ull, 0x719ec896ddc6ebadull, 0xf8f96f842eefb4caull, 0xa5ff8c2ed312fe7eull, 
        0xddf96d8f114f0a3cull, 0x32628b77adce7b2cull, 0xeafe008777a383fdull, 0xa0dce34aaad4f065ull, 0x946e4322d91dcc5bull, 0xd72a20b1dd3e1b9bull, 0x7799f4752340d3c4ull, 0x4f052da9ab027a03ull, 
        0x92457000ba4b0450ull, 0x9ce3fdbee620574bull, 0x7f22f22c02690f5aull, 0xf74bb358dd722a0full, 0xb5bd3ab7f508e0deull, 0x920032ad9ab809c8ull, 0x92447dc10995585bull, 0xd02ded4bcc963951ull, 
        0x9ea0ba546818147bull, 0x04cabb3a4b8ca853ull, 0xb82cd6415305cafeull, 0xb0bc221653bb1346ull, 0x08366924c7ea707full, 0x1c102b6cee87c50bull, 0x3304ceedb4e1ff7cull, 0xc95704c71074aa00ull, 
        0xd3a1bd1983263f2eull, 0xd5602d3e61ebcdb6ull, 0x5859765bd2d9f533ull, 0x162bedb8a31c33a8ull, 0xeed64921df140cfdull, 0x42f5c21e2b7a138bull, 0x5c85be2a1603d57cull, 0xcd5b1659d659e00full, 
        0xb3be712c23580aabull, 0x85d5e6e89b7dfcaeull, 0x01c5e2c1fba3c347ull, 0xb27a4dfa809e4bafull, 0x600e7290f7b4ba81ull, 0xca906c9c8426d587ull, 0x988191c301966407ull, 0x04f771e42caa89a3ull, 
        0xcd1b4bcdbf59e8fbull, 0x85b0c00eea64a320ull, 0xba70e2aae0b5da46ull, 0xc2160f372686ac76ull, 0xbcd63003e62447b7ull, 0x6c52924474cb00d1ull, 0x4b487b79aef7eb97ull, 0x714daa308fe0bbfbull, 
        0xdaf436844a3017f3ull, 0xb9061a71ff42f6ceull, 0x17c046e3c904460eull, 0x091b92270f0fa295ull, 0xcc63f5733ebbe1d8ull, 0x4fbe77229ea89ba7ull, 0x7f23267e07adae61ull, 0x5422754017493727ull, 
    }
};

// en passant
const std::uint64_t en_passant_rn[8] = {
    0x2f13dda13cc3d150ull, 0x0934611810430ed3ull, 0x3765067939839c72ull, 0xcdd2ccb3847a1e82ull, 0xd8bb9e70a19ad131ull, 0x04e9f7bde8c09fbbull, 0xed4e2514fa0b625cull, 0xc341101752f5f3caull, 
};

// castle rights
const std::uint64_t castle_rights_rn[4] = {
    // white kingside      white queenside        black kingside         black queenside
    0x931195e8d6ae0cc0ull, 0xa5616c48d4e934d5ull, 0xc438223a93dbaae1ull, 0xfcc285fc0790aa1full
};

// on-move is black
const std::uint64_t onmove_black_rn = 0xd11e2bbeba2f6d64ull;

std::uint64_t Board::zobristHash() const {
    std::uint64_t zob{0};
    for ( short rank = R8; rank >= R1; --rank ) {
        for ( short file = Fa; file <= Fh; ++file ) {
            PiecePtr ptr = at(rank,file);
            if ( !ptr->is_empty() ) {
                int typ = ptr->type() - 1;          // 0-5
                zob ^= pieces_rn[ptr->color()][typ][ptr->square().ordinal()];
            }
        }
    }

    for (byte bit = 0; bit < 4; ++bit) {
        if ( has_castle_right( bit ) )
            zob ^= castle_rights_rn[bit];
    }

    zob ^= castle_rights_rn[get_all_castle_rights()];

    if ( has_en_passant() )
        zob ^= en_passant_rn[get_en_passant().rank()];

    if ( get_on_move() == BLACK)
        zob ^= onmove_black_rn;

    return zob;
}
