//
// Created by aurailus on 2020-02-14.
//

#pragma once

#include <vector>
#include <type_traits>

struct RIE {
    template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type> static T read(
            const unsigned int ind, const std::vector<T>& data, const unsigned int dataLen) {

        if (ind >= dataLen) return 0;
        for (unsigned int i = 0; i < data.size(); i += 2) {
            if (data[i] > ind) return data[i - 1];
        }

        return data[data.size() - 1];
    }

    template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type> static bool write(
            const unsigned int ind, const T val, std::vector<T>& data, const unsigned int dataLen) {

        // Automatically keep the vectors in sane ranges
        if (data.capacity() < data.size() + 8) data.reserve(data.size() + 50);
        if (data.capacity() > data.size() + 60) data.shrink_to_fit();

        if (ind >= dataLen) return false;
        if (read(ind, data, dataLen) == val) return false;

        if (ind == 0) {
            if (data.size() == 0) {
                data[0] = 0;
                data[1] = val;
                return true;
            }
            else if (data.size() == 2) {
                data.push_back(1);
                data.push_back(data[1]);
                data[1] = val;
                return true;
            }
            else if (data[2] == ind + 1) {
                data[1] = val;
                return true;
            }
            else {
                data.insert(data.begin() + 2, 2, ind);
                data[2] = 1;
                data[3] = data[1];
                data[1] = val;
                return true;
            }
        }

        for (unsigned int i = 0; i < data.size(); i += 2) {
            if (data[i] == ind) {
                // We found an index equating to the block we are going to be setting.
                if (data[i - 1] == val) {
                    // The last block strip is the same block ID as what we are setting,
                    // So we should extend that strip.
                    if (data.size() > i + 2 && data[i + 2] == ind + 1) {
                        // The next block is one later, meaning we can simply remove this found block
                        // To cause the next strip to cascade over its position.
                        data.erase(data.begin() + i, data.begin() + i + 2);
                        return true;
                    }
                    else {
                        // The next strip is multiple blocks over, so just add one to our found block index.
                        data[i] += 1;
                        return true;
                    }
                }
                else {
                    // The last strip is not the same block.
                    if (data.size() > i + 2 && data[i + 2] == ind + 1) {
                        // There is only one of our block, so we can just update its id.
                        data[i + 1] = val;
                        return true;
                    }
                    else {
                        // The next strip is multiple blocks over, so we need to copy the previous block to the right
                        // and then set our block into its place
                        data.insert(data.begin() + i, 2, ind);
                        data[i + 1] = val;
                        data[i + 2] = ind + 1;
                        return true;
                    }
                }
            }
            else if (data[i] > ind) {
                // We found a strip with an index *larger* than our ind.
                // We can assume the last strip is not our block, because the getBlock() catch would have caught that.
                if (data[i] == ind + 1) {
                    if (data[i + 1] == val) {
                        // The next block over is the same, so we can just decrement our index by one.
                        data[i]--;
                        return true;
                    }
                    else {
                        // There is only one of our block to be placed, directly before our current strip
                        data.insert(data.begin() + i, 2, ind);
                        data[i + 1] = val;
                        return true;
                    }
                }
                else {
                    // The next strip is multiple blocks over, so we need to insert both our block
                    // *and* the previous strip's block after
                    data.insert(data.begin() + i, 4, ind);
                    data[i + 1] = val;
                    data[i + 2] = ind + 1;
                    data[i + 3] = data[i - 1];
                    return true;
                }
            }
        }
        // Escaped the for loop, meaning there's no index greater than ours.
        // We will insert our index at the end of the array, and insert the previous block after
        // if we're not at the end of the chunk.
        data.push_back(ind);
        data.push_back(val);

        if (ind >= dataLen - 1) return true; // Don't add the reset if at the end of the chunk.

        data.push_back(ind + 1);
        data.push_back(data[data.size() - 4]);
        return true;
    }
};