#pragma once
#include "ProtoWriter.h"
#include "ProtobuffMessages.h"

#define _gc2ch MatchmakingGC2ClientHello
#define _pci PlayerCommendationInfo
#define _pri PlayerRankingInfo

static std::string profile_info_changer(void *pubDest, uint32_t *pcubMsgSize) {
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 19);

	//replace commends
	auto _commendation = msg.has(_gc2ch::commendation) ? msg.get(_gc2ch::commendation).String() : std::string("");
	ProtoWriter commendation(_commendation, 4);
	commendation.replace(Field(_pci::cmd_friendly, TYPE_UINT32, (int64_t)SETTINGS::settings.RankChanger.cmd_friendly));
	commendation.replace(Field(_pci::cmd_teaching, TYPE_UINT32, (int64_t)SETTINGS::settings.RankChanger.cmd_teaching));
	commendation.replace(Field(_pci::cmd_leader, TYPE_UINT32, (int64_t)SETTINGS::settings.RankChanger.cmd_leader));
	msg.replace(Field(_gc2ch::commendation, TYPE_STRING, commendation.serialize()));

	//replace ranks
	auto _ranking = msg.has(_gc2ch::ranking) ? msg.get(_gc2ch::ranking).String() : std::string("");
	ProtoWriter ranking(_ranking, 6);
	ranking.replace(Field(_pri::rank_id, TYPE_UINT32, (int64_t)SETTINGS::settings.RankChanger.rank_id));
	ranking.replace(Field(_pri::wins, TYPE_UINT32, (int64_t)SETTINGS::settings.RankChanger.wins));
	msg.replace(Field(_gc2ch::ranking, TYPE_STRING, ranking.serialize()));

	//replace private level
	msg.replace(Field(_gc2ch::player_level, TYPE_INT32, (int64_t)SETTINGS::settings.RankChanger.player_level));
//	msg.replace(Field(_gc2ch::player_level, TYPE_INT32, (int64_t)DreamHack.RankChanger.player_level_xp)); too much bag so 
	//bans
	if (!SETTINGS::settings.RankChanger.ban == 0 && !SETTINGS::settings.RankChanger.time == 0)
	{
		msg.replace(Field(_gc2ch::penalty_reason, TYPE_INT32, (int64_t)SETTINGS::settings.RankChanger.ban));
		msg.replace(Field(_gc2ch::penalty_seconds, TYPE_INT32, (int64_t)SETTINGS::settings.RankChanger.time));
	}
	


	return msg.serialize();
}