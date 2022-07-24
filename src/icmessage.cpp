#include "icmessage.h"

/**
 * @brief This enum acts as an association between an AO Network Protocol IC
 * message's contents' indices and what they mean.
 *
 * @details Yes, it's as cursed as it looks. I'll try easing the understanding
 * of this shit by copying over the comments from
 * https://github.com/in1tiate/kenji-docs/blob/master/docs/development/network.md#in-character-message.
 * Will that help? Maybe not.
 *
 * Since I don't want this to be actually used in any of the code beyond this
 * "parser" part, I have purposefully not defined this in the header.
 *
 * @notes oh god oh fuck
 */
enum CHAT_MESSAGE {
  DESK_MOD = 0,      /*!<
                      * @var CHAT_MESSAGE::DESK_MOD
                      *
                      * Whether or not to override desk appearance.
                      *
                      * - chat: Positions "def", "pro", and "wit" default to desk and
                      * the positions "hld", "hlp" and "jud" to no desk.
                      * -- 0: desk is hidden
                      * -- 1: desk is shown
                      * -- 2: desk is hidden during preanim, shown when it ends
                      * - 3: desk is shown during preanim, hidden when it ends
                      * - 4: desk is hidden during preanim, character is centered and
                      * pairing is ignored, when it ends desk is shown and pairing is
                      * restored
                      * - 5: desk is shown during preanim, when it ends character is
                      * centered and pairing is ignored
                      */
  PRE_EMOTE,         /*!<
                      * The animation that plays before the character starts talking.
                      * Does not include file extension.
                      */
  CHAR_NAME,         /*!< The folder name of the character that is talking. */
  EMOTE,             /*!<
                      * The emote that should play. Does not include (a)/(b) prefixes or
                      * file extensions.
                      */
  MESSAGE,           /*!<
                      * The chat message as to be displayed in the chatbox and the IC
                      * log. Note that the message may contain markup that must be parsed.
                      */
  SIDE,              /*!<
                      * Which side the character is on. See the character authoring page
                      * for valid positions.
                      */
  SFX_NAME,          /*!<
                      * Name of the sound effect that should play during the preanimation
                      * (if the preanimation is enabled).
                      */
  EMOTE_MOD,         /*!<
                      * A number that dictates emote behavior:
                      * - 0: do not play preanimation; overridden to 2 by a non-0
                      * objection modifier
                      * - 1: play preanimation (and sfx)
                      * - 2: play preanimation and play objection
                      * - 3: unused
                      * - 4: unused
                      * - 5: no preanimation and zoom
                      * - 6: objection and zoom, no preanim
                      */
  CHAR_ID,           /*!<
                      * Character identifier; dictates the index of the character in the
                      * character list (starting from zero).
                      */
  SFX_DELAY,         /*!<
                      * Dictates how long in milliseconds the client should wait after
                      * the preanimation has started playing before playing the
                      * associated sound effect.
                      */
  OBJECTION_MOD,     /*!<
                      * Dictates if the player uses a shout.
                      * - 0: nothing
                      * - 1: "Hold it!"
                      * - 2: "Objection!"
                      * - 3: "Take that!"
                      * - 4&{name}: custom shout (since 2.8)
                      */
  EVIDENCE_ID,       /*!<
                      * ID of the evidence presented. 0 is no evidence presented, so
                      * evidence ID effectively starts from 1.
                      */
  FLIP,              /*!<
                      * Dictates if the emote should be flipped.
                      * - 0: no flip
                      * - 1: flip
                      */
  REALIZATION,       /*!<
                      * Dictates whether a realization flash and sound
                      * effect should play or not.
                      * - 0: no realization
                      * - 1: realization
                      */
  TEXT_COLOR,        /*!<
                      * text_color: Dictates which color the text of the chat message
                      * should be.
                      * - 0: white
                      * - 1: green
                      * - 2: red
                      * - 3: orange
                      * - 4: blue (disables talking animation)
                      * - 5: yellow
                      * - 6: rainbow (removed in 2.8)
                      */
  SHOWNAME,          /*!<
                      * If used, this will show a custom name (showname) for the
                      * character.
                      */
  OTHER_CHARID,      /*!<
                      * The character ID of the person the player wishes to pair up
                      * with.
                      */
  OTHER_NAME,        /*!<
                      * The folder name of the character the player is pairing up with
                      * (in case of INI swapping).
                      */
  OTHER_EMOTE,       /*!<
                      * The emote the user's pair was doing. Note that by default,
                      * zooms (that are correctly defined as such) do not update this
                      * value, so a pair of zooms will not appear. Zooms also enjoy
                      * special privileges, in that (assuming they are correctly
                      * defined, again) they make the pair disappear in the client and
                      * get centered.
                      */
  SELF_OFFSET,       /*!<
                      * the percentage by which the character is shifted horizontally,
                      * from -100   (one whole screen's worth to the left) to 100 (one
                      * whole screen's worth to   the right). This parameter also
                      * stores   vertical offset, which is   self-explanatory.
                      * - {x_offset}: <2.9
                      * - {x_offset}&{y_offset}: 2.9+
                      */
  OTHER_OFFSET,      /*!<
                      * The user's pair's self_offset, basically.
                      */
  OTHER_FLIP,        /*!<
                      * The user's pair's char_id2/flip, basically.
                      */
  IMMEDIATE,         /*!<
                      * If 1, the text begins at the same time as the preanimation.
                      */
  LOOPING_SFX,       /*!<
                      * If 1, the sound effect loops until another emote is played.
                      */
  SCREENSHAKE,       /*!<
                      * If 1, the screen shakes (TODO: on preanimation or on chat?).
                      */
  FRAME_SCREENSHAKE, /*!<
                      * A list of frames for which the screen should shake
                      * (TODO: list format).
                      */
  FRAME_REALIZATION, /*!<
                      * A list of frames for which the screen should flash
                      * (TODO: list format).
                      */
  FRAME_SFX,         /*!<
                      * A list of frames for which the sound effect should play.
                      */
  ADDITIVE,          /*!<
                      * If 1, does not clear the chatbox's previous message.
                      */
  EFFECTS            /*!<
                      * The overlay effect to be displayed.
                      */
};

std::optional<ICMessage> ICMessage::from(const AOPacket &f_packet)
{
  const QStringList &l_contents = f_packet.get_contents();

  if ("MS" != f_packet.get_header()) {
    return std::nullopt;
  }

  if (0 > l_contents[CHAR_ID]) {
    return std::nullopt;
  }

  // TODO: Replace
  return std::nullopt;
}

AOPacket ICMessage::to_packet() const
{ // TODO Add
  return AOPacket("MS");
}
