#include "KTRoboMessage.h"
#include "KTRoboCS.h"

using namespace KTROBO;
Message::Message(void)
{

}


Message::~Message(void)
{
}

int Messages::makeInst() {
	
	if (messages.size()) return 0;
	Message* n = new Message();
	messages.push_back(n);
	return 0;
}
Message* Messages::getInstance(int index) {
	if (messages.size()) {
		return messages[0];
	} else {
		makeInst();
		return messages[0];
	}
}

IMessage* Messages::getInterface(int index) {
	if (messages.size()) {
		return messages[0];
	} else {
		makeInst();
		return messages[0];
	}
}


void Message::makeMessage(int msg_id, int sender_id, int receiver_id, int msg, float fmsg, bool is_need_to_send) {
	
	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	for (int i=0;i<KTROBO_MESSAGE_MYMSTRUCT_SIZE;i++) {
	
		if (!structs[i].is_use) {
			structs[i].msg = msg;
			structs[i].fmsg = fmsg;
			structs[i].msg_id = msg_id;
			structs[i].sender_id = sender_id;
			structs[i].receiver_id = receiver_id;
			structs[i].is_need_to_send = is_need_to_send;
			structs[i].is_use = true;
			CS::instance()->leave(CS_MESSAGE_CS, "leave");
			return;
		}
	}

	for (int i=0;i<KTROBO_MESSAGE_MYMSTRUCT_SIZE;i++) {
	
		if (structs[i].is_use && !structs[i].is_need_to_send) {
			structs[i].msg = msg;
			structs[i].fmsg = fmsg;
			structs[i].msg_id = msg_id;
			structs[i].sender_id = sender_id;
			structs[i].receiver_id = receiver_id;
			structs[i].is_need_to_send = is_need_to_send;
			structs[i].is_use = true;
			CS::instance()->leave(CS_MESSAGE_CS, "leave");
			return;
		}
	}
	// 入れられる場所がない
	if (is_need_to_send) {
		// どうしても入れる必要があるメッセージの場合は例外を投げる
		CS::instance()->leave(CS_MESSAGE_CS, "leave");
		throw new GameError(KTROBO::WARNING, "no message irerubasho");
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave");
}

int Message::getMessageIndexsFromMsgId(int msg_idm, OUT_ int* get_msgs) {
	int temp_count=0;
	int ans_count=0;
	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	for (int i=0;i<KTROBO_MESSAGE_MYMSTRUCT_SIZE && temp_count <TEMPHAIRETU_SIZE;i++) {
	
		if (structs[i].is_use) {
			if (structs[i].msg_id == msg_idm) {
				if (temp_count < TEMPHAIRETU_SIZE) {
					get_msgs[temp_count] = i;
					temp_count++;
					structs[i].is_need_to_send = true; // 書き換えられるのをふせぐためtrueにする
				}
				ans_count++;
			}
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave");
	return ans_count;
}


int Message::getMessageIndexsFromReceiverId(int receiver_id, OUT_ int* get_msgs) {
	int temp_count=0;
	int ans_count =0;
	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	for (int i=0;i<KTROBO_MESSAGE_MYMSTRUCT_SIZE;i++) {
	
		if (structs[i].is_use) {
			if (structs[i].receiver_id == receiver_id) {
				if (temp_count < TEMPHAIRETU_SIZE) {
					get_msgs[temp_count] = i;
					temp_count++;
					structs[i].is_need_to_send = true; // 書き換えられるのをふせぐためtrueにする
			
				}
				ans_count++;
			}
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave");
	return ans_count;
}


void Message::getMsgBody(int mindex_len, int* msg_indexs, OUT_ int* msg_ids, OUT_ int* sender_ids, OUT_ int* receiver_ids, OUT_ int* msgs, OUT_ float* fmsgs) {
	// msgsは要素数32以下の配列にすること
	if (mindex_len > TEMPHAIRETU_SIZE) {
		throw new GameError(KTROBO::WARNING, "error in getmsgbody");
	}

	CS::instance()->enter(CS_MESSAGE_CS, "enter");
	for (int i=0;i<mindex_len;i++) {
		int inde = msg_indexs[i];
		if (structs[inde].is_use) {
			msg_ids[i] = structs[inde].msg_id;
			sender_ids[i] = structs[inde].sender_id;
			receiver_ids[i] = structs[inde].receiver_id;
			msgs[i] = structs[inde].msg;
			fmsgs[i] = structs[inde].fmsg;
		} else {
			CS::instance()->leave(CS_MESSAGE_CS, "leave");
			throw new GameError(KTROBO::WARNING, "try to access nousestruct");
		}
	}

	for (int i=0;i<mindex_len;i++) {
		int inde = msg_indexs[i];
		if (structs[inde].is_use) {
			structs[inde].is_use = false;
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave");
}