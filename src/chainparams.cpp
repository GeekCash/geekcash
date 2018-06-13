// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 The GeekCash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

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
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "GeekCash - Don't work for weekends, work for our goals.";
    const CScript genesisOutputScript = CScript() << ParseHex("04716b72451310faf205414ebb80650e286573e6554a290a1280d1955d772c6ea602d033f1f76fbd099bacba8156af85428a0197c0a3d6f0a79f8bb575c4c16751") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
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
        consensus.nSubsidyHalvingInterval = 262800; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 180000; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        // not use
        consensus.nMasternodePaymentsIncreaseBlock = 158000; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - actual historical value
        // 
        
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 300000; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 43200; // actual number of blocks per month 1440 * 30 = 43200
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24*7; // 7 days
        consensus.nSuperblockStartBlock = 614820; // The block at which 12.1 goes live (end of final 12.0 budget cycle)
        consensus.nSuperblockCycle = 43200; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x000007de1b1fb05d656af7b75b398efff9e9562ab73047750394527f0f239e62");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 1 * 60 * 60; // GeekCash: 1 blocks
        consensus.nPowTargetSpacing = 60; // GeekCash: 1 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 1;
        consensus.nPowDGWHeight = 1; // DarkGravityWave
        consensus.nRuleChangeActivationThreshold = 3240; // 90% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 3600; // nPowTargetTimespan / nPowTargetSpacing * 6
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1508025600; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1539561600; // Oct 15th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.        
        consensus.nMinimumChainWork = uint256S("000000000000000000000000000000000000000000000000002c51c8e4beb570"); // 46789

        // By default assume that the signatures in ancestors of this block are valid.        
        consensus.defaultAssumeValid = uint256S("0x000007de1b1fb05d656af7b75b398efff9e9562ab73047750394527f0f239e62"); // genesis

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xb8;
        pchMessageStart[1] = 0x6c;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0xb8;
        vAlertPubKey = ParseHex("049a57e861057265b593ab35ab8f6e4862303b086879d6680fd203f69ceab4d0e643a8862b5ae779d8ddf37ccea3c543b8e394aa75e1df2a23eec16475beda7a07");
        nDefaultPort = 6889;
        nMaxTipAge = 24 * 60 * 60; // 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1525796559, 617415, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // printf("-----main\n");
        // printf("genesis hash: %s\n", consensus.hashGenesisBlock.ToString().c_str());
        // printf("root hash: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        // printf("%s\n", genesis.ToString().c_str());        
        assert(consensus.hashGenesisBlock == uint256S("0x000007de1b1fb05d656af7b75b398efff9e9562ab73047750394527f0f239e62"));
        assert(genesis.hashMerkleRoot == uint256S("0x6d87016979d2f369dcb5fc3a5c284be1a316790cbaabfcce403d24da4b49b210"));


        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns01.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns02.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns03.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns04.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns05.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns06.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns07.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns08.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns09.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns10.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns11.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns12.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns13.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns14.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns15.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns16.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns17.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns18.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns19.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns20.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns21.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns22.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns23.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns24.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("geekcash.org", "ns25.geekcash.org"));
        
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns01.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns02.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns03.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns04.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns05.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns06.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns07.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns08.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns09.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns10.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns11.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns12.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns13.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns14.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns15.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns16.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns17.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns18.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns19.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns20.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns21.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns22.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns23.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns24.geekcash.net"));
        vSeeds.push_back(CDNSSeedData("geekcash.net", "ns25.geekcash.net"));

        // GeekCash addresses start with 'G'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
        // GeekCash script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19); //76
        // GeekCash private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        // GeekCash BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // GeekCash BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // GeekCash BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "042946f6119c7257ada82d20c6032a9468e3482d02785a1437e4271bd887b234b2fe1760f9f5a6fcac082d3254f384148a11f28e9a332f7ec0c87d01c477341e0d";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of            
            ( 0, uint256S("0x000007de1b1fb05d656af7b75b398efff9e9562ab73047750394527f0f239e62"))
            ( 20800, uint256S("0x0000000009aeb38c5be87990fd7f1a38e90a641f61310000ab17fdcb973c86f3"))
            ( 46789, uint256S("0x0000000000412e9f847e6f1e331ccb36084946effe35386a4eeee3216d5bf079")),           
            1528767182, // * UNIX timestamp of last checkpoint block
            62493,    // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            5000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0000012534fc6859842d91daca14a8bb8254ae4d061debf3cb5ea8dcbcdbbea9");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 1 * 60 * 60; // GeekCash: 1 day
        consensus.nPowTargetSpacing = 1 * 60; // GeekCash: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 4001;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010"); // 37900

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0000012534fc6859842d91daca14a8bb8254ae4d061debf3cb5ea8dcbcdbbea9"); // 37900

        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xe2;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0xff;
        vAlertPubKey = ParseHex("04ae93efe06fc650fc894bec5d48929878fd7224db7894a6b4f3ef702ae5b38f32538777d64b9139ec547e6aef1cef3329c4174822777382c1baad0541234ab717");
        nDefaultPort = 16889;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1523521813, 325703, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // printf("-----testnet\n");
        // printf("genesis hash: %s\n", consensus.hashGenesisBlock.ToString().c_str());
        // printf("root hash: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        // printf("%s\n", genesis.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0000012534fc6859842d91daca14a8bb8254ae4d061debf3cb5ea8dcbcdbbea9"));
        assert(genesis.hashMerkleRoot == uint256S("6d87016979d2f369dcb5fc3a5c284be1a316790cbaabfcce403d24da4b49b210"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testnet.geekcash.org",  "ns01.testnet.geekcash.org"));
        vSeeds.push_back(CDNSSeedData("testnet.geekcash.org",  "ns02.testnet.geekcash.org"));

        // Testnet GeekCash addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        // Testnet GeekCash script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,76); // 19
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet GeekCash BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet GeekCash BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet GeekCash BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "042946f6119c7257ada82d20c6032a9468e3482d02785a1437e4271bd887b234b2fe1760f9f5a6fcac082d3254f384148a11f28e9a332f7ec0c87d01c477341e0d";

        // checkpointData = (CCheckpointData) {
        //     boost::assign::map_list_of
        //     (    261, uint256S("0x00000c26026d0815a7e2ce4fa270775f61403c040647ff2c3091f99e894a4618"))
        //     (   1999, uint256S("0x00000052e538d27fa53693efe6fb6892a0c1d26c0235f599171c48a3cce553b1"))
        //     (   2999, uint256S("0x0000024bc3f4f4cb30d29827c13d921ad77d2c6072e586c7f60d83c2722cdcc5")),

        //     1462856598, // * UNIX timestamp of last checkpoint block
        //     3094,       // * total number of transactions between genesis and last checkpoint
        //                 //   (the tx=... number in the SetBestChain debug.log lines)
        //     500         // * estimated number of transactions per day after checkpoint
        // };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; // GeekCash: 1 minutes
        consensus.nPowTargetSpacing = 60; // GeekCash: 60 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 34140; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 19994;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1518686868, 1096447, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // printf("-----regtest\n");
        // printf("genesis hash: %s\n", consensus.hashGenesisBlock.ToString().c_str());
        // printf("root hash: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        // printf("%s\n", genesis.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("ae02dbb3cfa4ff51e21a6291e3489b8d63c9d5e3353a1fa8dc6bd4381b38e946"));
        assert(genesis.hashMerkleRoot == uint256S("6d87016979d2f369dcb5fc3a5c284be1a316790cbaabfcce403d24da4b49b210"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        // checkpointData = (CCheckpointData){
        //     boost::assign::map_list_of
        //     ( 0, uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e")),
        //     0,
        //     0,
        //     0
        // };
        // Regtest GeekCash addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Regtest GeekCash script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest GeekCash BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest GeekCash BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest GeekCash BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
