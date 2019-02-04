// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "2018, June 7th. A paper medium passed away in Japan, and the MANGA era began.";
    const CScript genesisOutputScript = CScript() << OP_DUP << OP_HASH160 << ParseHex("d2034f9e9ad09b1b32105a6efa47e19fdbabdfe4") << OP_EQUALVERIFY << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 700979;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("e0b0b95cc209e17dd4280e3ab830256783d89ef714accce540232da33e2b320a");
        consensus.BIP65Height = -1;
        consensus.BIP66Height = -1;
        consensus.powLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 30 * 60; //  30 minutes
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigishield = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7560; // 75% of 10080
        consensus.nMinerConfirmationWindow = 10080; // 3.5 days / nPowTargetSpacing * 4 * 0.75
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1532185200; // 2018-07-22 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1537542000; // 2018-09-22 00:00:00

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1532185200; // 2018/07/22 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1537542000; // 2018-09-22 00:00:00

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("00000000000000000000000000000000000000000000000000bbd2a540b18171"); //60000

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xe0b0b95cc209e17dd4280e3ab830256783d89ef714accce540232da33e2b320a"); //0

        consensus.nSubsidyAmount = 10714 * COIN;
        consensus.nSubsidyBlankHeight = 2 * 24 * 3600 / consensus.nPowTargetSpacing; //2 days

        // Hardfork params
        nSwitchKGWblock = -1;
        nSwitchDIGIblock = -1;
        nSwitchLyra2REv2_DGW = 1;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x12;
        pchMessageStart[1] = 0x34;
        pchMessageStart[2] = 0x56;
        pchMessageStart[3] = 0x78;
        nDefaultPort = 18730;
        nPruneAfterHeight = 100000;
        vAlertPubKey[MAIN_KEY] = ParseHex("04a249957c97bd85571858786e6383d1e9b6d323ecad813b63cb52c8da86d56def2e9974dbb5c1c749aa9cf34f3f175aba43e22898f176eceec591f045b1d37a68");
        vAlertPubKey[SUB_KEY]  = ParseHex("04a249957c97bd85571858786e6383d1e9b6d323ecad813b63cb52c8da86d56def2e9974dbb5c1c749aa9cf34f3f175aba43e22898f176eceec591f045b1d37a68");

        genesis = CreateGenesisBlock(1532145131, 0xE918, 0x1F0FFFFF, 1, 3500000358212617622);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xe0b0b95cc209e17dd4280e3ab830256783d89ef714accce540232da33e2b320a"));
        assert(genesis.hashMerkleRoot == uint256S("0x7f7fd8e22ea946489fedb1151fdc397dfc666d474654edc155f07fec6ee64bb2"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("dnsseed.manga-core.com", false);
        vSeeds.emplace_back("dnsseed.fthservice.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,21);  // N
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,15);  // G
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,15);  // G
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,115);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x1e, 0xb2, 0x88, 0x04};
        base58Prefixes[EXT_SECRET_KEY] = {0xe4, 0xad, 0x88, 0x04};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,115);

        bech32_hrp = "arkr"; // Argon and Krypton

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {   0, uint256S("0xe0b0b95cc209e17dd4280e3ab830256783d89ef714accce540232da33e2b320a")},
                {1000, uint256S("0x68bbbc087c8e1bb74408e95a9752ac6587fa95e2ef151e4da1829defda557dc4")},
                {3000, uint256S("0x03244c04dcca0be9f4cb94cf15a0bfa3c8e35cb0793862cb71dd8de0b0927298")},
                {5000, uint256S("0x1d8cc09f798eeba22d1a199136d05b8ba80c95410e7a567546d7ddcccfc17a2d")},
                {7000, uint256S("0x54a4f69da2152547b0b1cf94ca216c2b244fc871fd949feda7b549f3dd6e5cfe")},
                {9000, uint256S("0xe0299f12702b7da5e86be95f879f7842a895ee3842b85c5010477c269afc3a37")},
                {10000, uint256S("0x22743d431cac4ae66ffcb29a27ee294fd862202490b9ecf440de55bc9aba7cef")},
                {20000, uint256S("0xe26b767c727e8f98666a7ddec02d7c6d494891f9c9f160d28ff62add858eb8e8")},
                {30000, uint256S("0xaefc30184882696425353b0cb1aa20ff912d0c0b58a574cb6c2a86f210589247")},
                {40000, uint256S("0x21b02f0e2336d5a8193b1afd2defa0cee11c05e11c967e2473a66acee23312d6")},
                {50000, uint256S("0x8109aaaa785f413259cffb4855dcd270c980b22bfb23c0d2dd3f68136e330067")},
                {60000, uint256S("0xf0e4d8880f25b4a12cd637649e50b5451517e5b5a97761067d1157578ab87de6")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block f8c409c2dd5b84db7c9d69ce5209261854d42da26b1f78c17d8b23aba2f5efc3 (height 60000).
            1538307671, // * UNIX timestamp of last known number of transactions
            92049,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.01493686    // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 3000010;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("39c55d04ea8b5117abb84809c862a9d17212bdae22115bef80f6a508f2a24ab6");
        consensus.BIP65Height = -1;
        consensus.BIP66Height = -1;
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigishield = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 75; // 75% for testchains
        consensus.nMinerConfirmationWindow = 100; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1531837800; // 2018-07-17 23:30:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1531873800; // 2018-07-18 09:30:00
 
        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1531837800; // 2018-07-17 23:30:00
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1531873800; // // 2018-07-18 09:30:00
        // The best chain should have at least this much work.
//        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000001b57ceb7b646");
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000007a577bc240");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x39c55d04ea8b5117abb84809c862a9d17212bdae22115bef80f6a508f2a24ab6"); //160675

        consensus.nSubsidyAmount = 2500 * COIN;
	    consensus.nSubsidyBlankHeight = 30 * 60 / consensus.nPowTargetSpacing; //30 minutes

        pchMessageStart[0] = 0xab;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0xef;
        pchMessageStart[3] = 0x99;

        // Hardfork params
        nSwitchKGWblock = -1;
        nSwitchDIGIblock = -1;
        nSwitchLyra2REv2_DGW = 1;

        nDefaultPort = 23730;
        nPruneAfterHeight = 1000;
        vAlertPubKey[MAIN_KEY] = ParseHex("04a249957c97bd85571858786e6383d1e9b6d323ecad813b63cb52c8da86d56def2e9974dbb5c1c749aa9cf34f3f175aba43e22898f176eceec591f045b1d37a68");
        vAlertPubKey[SUB_KEY]  = ParseHex("04a249957c97bd85571858786e6383d1e9b6d323ecad813b63cb52c8da86d56def2e9974dbb5c1c749aa9cf34f3f175aba43e22898f176eceec591f045b1d37a68");

        genesis = CreateGenesisBlock(1531637354, 0xE1E6, 0x2000FFFF, 1, 3500000250039000130);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x39c55d04ea8b5117abb84809c862a9d17212bdae22115bef80f6a508f2a24ab6"));
        assert(genesis.hashMerkleRoot == uint256S("0x8f85a3537367aa9a83bec83d70d1a71a54e79354bf875afd96a211d1754ab66e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("dnsseed.manga-core.com", false);
        vSeeds.emplace_back("dnsseed.fthservice.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,19);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,233);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xcf, 0x87, 0x35, 0x04};
        base58Prefixes[EXT_SECRET_KEY] = {0x94, 0x83, 0x35, 0x04};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,233);

        bech32_hrp = "thene"; // Test HElium NEon

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x39c55d04ea8b5117abb84809c862a9d17212bdae22115bef80f6a508f2a24ab6")},
                {1942, uint256S("0x082beec281f3e7d2eb2dc4400baf526ee4b5713b27226c46fa9c83a61e84b0d9")}
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 4bf184706cb65e6571185b2dae8ee95783567ecd18cdc6c1506fc9f281c1bb6a (height 160000)
            1531837006,  //block height 1942
            1969,
            0.00986216
        };


    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = -1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = -1; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = -1; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1.1 * 24 * 60 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigishield = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Hardfork params
        nSwitchKGWblock = 20;
        nSwitchDIGIblock = 40;
        nSwitchLyra2REv2_DGW = 60;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 20444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 1, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7543a69d7c2fcdb29a5ebec2fc064c074a35253b6f3072c8a749473aa590a29c"));
//        assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = {
            {
                { 0, uint256S("3121f07c5791eea22d053e5a5fcfef0032a2ed38b434a7a53639c63b2ca74718")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);

        bech32_hrp = "rmona";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
