#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"


/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels//header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level__entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x7, __segment_7SegmentRomStart, __segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _bbh_skybox_mio0SegmentRomStart, _bbh_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario),

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, _area_1),
		WARP_NODE(0x0A, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_ENDING, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CASTLE_GROUNDS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BOO, -521, 304, 1566, 0, 0, 0, 0x00000000, bhvHeaveHo),
		OBJECT(MODEL_BOO, 1267, 296, 1149, 0, 0, 0, 0x00000000, bhvHeaveHo),
		OBJECT(MODEL_BOO, 1401, 282, -1341, 0, 0, 0, 0x00000000, bhvHeaveHo),
		OBJECT(MODEL_BOO, -421, 287, -1845, 0, 0, 0, 0x00000000, bhvHeaveHo),
		OBJECT(MODEL_BOO, -1899, 287, -12, 0, 0, 0, 0x00000000, bhvHeaveHo),
		OBJECT(MODEL_STAR, -2109, 1880, -1570, 0, 0, 0, 0x00000000, bhvStar),
		MARIO_POS(0x01, 0, 1608, 387, 66),
		MARIO_POS(0x01, 0, 1608, 387, 66),
		TERRAIN(_area_1_collision),
		MACRO_OBJECTS(_area_1_macro_objs),
		STOP_MUSIC(0),
		TERRAIN_TYPE(TERRAIN_SPOOKY),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	AREA(2, _area_2),
		OBJECT(MODEL_BLACK_BOBOMB, 1685, 262, 482, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_BLACK_BOBOMB, 1691, 196, -77, 0, 0, 0, 0x00000000, bhvGoomba),
		OBJECT(MODEL_WOODEN_SIGNPOST, 984, -126, -97, 0, 0, 0, 0x00000000, bhvSignOnWall),
		TERRAIN(_area_2_collision),
		MACRO_OBJECTS(_area_2_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 1608, 387, 66),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
